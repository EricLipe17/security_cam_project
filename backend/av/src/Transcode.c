#include "Transcode.h"

#include "AVUtil.h"

#include <libavutil/timestamp.h>
#include <libavutil/opt.h>

#include <string.h>

#define STORAGE_DIR "/tmp/singularity_output/"

////// Private methods
int init_output_filename(TranscodeContext** _pT)
{
    if (!(*_pT)->m_pOutParams->m_pFQP)
        free((*_pT)->m_pOutParams->m_pFQP);
    get_next_filename((*_pT)->m_pOutParams->m_pFN, (*_pT)->m_pOutParams->m_pOutExt, &(*_pT)->m_pOutParams->m_pFQN);

    const size_t nSizeDir = strlen(STORAGE_DIR);
    const size_t nSizeFQP = strlen((*_pT)->m_pOutParams->m_pFQN);

    (*_pT)->m_pOutParams->m_pFQP = malloc(sizeof(char) * (nSizeDir + nSizeFQP));
    stpncpy((*_pT)->m_pOutParams->m_pFQP, STORAGE_DIR, nSizeDir);
    stpncpy((*_pT)->m_pOutParams->m_pFQP + nSizeDir, (*_pT)->m_pOutParams->m_pFQN, nSizeFQP);
}

int open_media(const char* _pUrl, AVFormatContext** _ppFmtCtx)
{
    *_ppFmtCtx = avformat_alloc_context();
    if (!*_ppFmtCtx) {av_log(NULL, AV_LOG_ERROR, "failed to alloc memory for format"); return -1;}

    if (avformat_open_input(_ppFmtCtx, _pUrl, NULL, NULL) != 0) {av_log(NULL, AV_LOG_ERROR, "failed to open input file %s", _pUrl); return -1;}

    if (avformat_find_stream_info(*_ppFmtCtx, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to get stream info"); return -1;}
    return 0;
}

int init_video_encoder(StreamContext* _pCtx, AVCodecContext* _pDecoderCodecCtx, AVRational framerate, StreamingParams params)
{
    _pCtx->m_pVideoStream = avformat_new_stream(_pCtx->m_pFormatCtx, NULL);

    _pCtx->m_pVideoCodec = avcodec_find_encoder(params.m_nVideoID);
    if (!_pCtx->m_pVideoCodec) {av_log(NULL, AV_LOG_ERROR, "could not find the proper codec"); return -1;}

    _pCtx->m_pVideoCodecCtx = avcodec_alloc_context3(_pCtx->m_pVideoCodec);
    if (!_pCtx->m_pVideoCodecCtx) {av_log(NULL, AV_LOG_ERROR, "could not allocated memory for codec context"); return -1;}

    av_opt_set(_pCtx->m_pVideoCodecCtx->priv_data, "preset", "fast", 0);
    if (params.m_pCodecPrivKey && params.m_pCodecPrivVal)
        av_opt_set(_pCtx->m_pVideoCodecCtx->priv_data, params.m_pCodecPrivKey, params.m_pCodecPrivVal, 0);

    _pCtx->m_pVideoCodecCtx->height = _pDecoderCodecCtx->height;
    _pCtx->m_pVideoCodecCtx->width = _pDecoderCodecCtx->width;
    _pCtx->m_pVideoCodecCtx->sample_aspect_ratio = _pDecoderCodecCtx->sample_aspect_ratio;
    if (_pCtx->m_pVideoCodec->pix_fmts)
        _pCtx->m_pVideoCodecCtx->pix_fmt = _pCtx->m_pVideoCodec->pix_fmts[0];
    else
        _pCtx->m_pVideoCodecCtx->pix_fmt = _pDecoderCodecCtx->pix_fmt;

    _pCtx->m_pVideoCodecCtx->bit_rate = 2 * 1000 * 1000;
    _pCtx->m_pVideoCodecCtx->rc_buffer_size = 4 * 1000 * 1000;
    _pCtx->m_pVideoCodecCtx->rc_max_rate = 2 * 1000 * 1000;
    _pCtx->m_pVideoCodecCtx->rc_min_rate = 2.5 * 1000 * 1000;

    _pCtx->m_pVideoCodecCtx->time_base = av_inv_q(framerate);
    _pCtx->m_pVideoStream->time_base = _pCtx->m_pVideoCodecCtx->time_base;

    if (avcodec_open2(_pCtx->m_pVideoCodecCtx, _pCtx->m_pVideoCodec, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "could not open the codec"); return -1;}
    avcodec_parameters_from_context(_pCtx->m_pVideoStream->codecpar, _pCtx->m_pVideoCodecCtx);
    return 0;
}

int init_audio_encoder(StreamContext* _pCtx, int sample_rate, StreamingParams params)
{
    _pCtx->m_pAudioStream = avformat_new_stream(_pCtx->m_pFormatCtx, NULL);

    _pCtx->m_pAudioCodec = avcodec_find_encoder(params.m_nAudioID);
    if (!_pCtx->m_pAudioCodec) {av_log(NULL, AV_LOG_ERROR, "could not find the proper codec"); return -1;}

    _pCtx->m_pAudioCodecCtx = avcodec_alloc_context3(_pCtx->m_pAudioCodec);
    if (!_pCtx->m_pAudioCodecCtx) {av_log(NULL, AV_LOG_ERROR, "could not allocated memory for codec context"); return -1;}

    int OUTPUT_CHANNELS = 2;
    int OUTPUT_BIT_RATE = 196000;
    _pCtx->m_pAudioCodecCtx->channels       = OUTPUT_CHANNELS;
    _pCtx->m_pAudioCodecCtx->channel_layout = av_get_default_channel_layout(OUTPUT_CHANNELS);
    _pCtx->m_pAudioCodecCtx->sample_rate    = sample_rate;
    _pCtx->m_pAudioCodecCtx->sample_fmt     = _pCtx->m_pAudioCodec->sample_fmts[0];
    _pCtx->m_pAudioCodecCtx->bit_rate       = OUTPUT_BIT_RATE;
    _pCtx->m_pAudioCodecCtx->time_base      = (AVRational){1, sample_rate};

    _pCtx->m_pAudioCodecCtx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    _pCtx->m_pAudioStream->time_base = _pCtx->m_pAudioCodecCtx->time_base;

    if (avcodec_open2(_pCtx->m_pAudioCodecCtx, _pCtx->m_pAudioCodec, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "could not open the codec"); return -1;}
    avcodec_parameters_from_context(_pCtx->m_pAudioStream->codecpar, _pCtx->m_pAudioCodecCtx);
    return 0;
}

int fill_stream_info(AVStream* _pStream, const AVCodec** _ppCodec, AVCodecContext** _ppCodecCtx) {
    *_ppCodec = avcodec_find_decoder(_pStream->codecpar->codec_id);
    if (!*_ppCodec) {av_log(NULL, AV_LOG_ERROR, "failed to find the codec"); return -1;}

    *_ppCodecCtx = avcodec_alloc_context3(*_ppCodec);
    if (!*_ppCodecCtx) {av_log(NULL, AV_LOG_ERROR, "failed to alloc memory for codec context"); return -1;}

    if (avcodec_parameters_to_context(*_ppCodecCtx, _pStream->codecpar) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to fill codec context"); return -1;}

    if (avcodec_open2(*_ppCodecCtx, *_ppCodec, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to open codec"); return -1;}
    return 0;
}

int get_stream_refs(StreamContext* _pInCtx)
{
    for (int i = 0; i < _pInCtx->m_pFormatCtx->nb_streams; i++)
    {
        if (_pInCtx->m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            _pInCtx->m_pVideoStream = _pInCtx->m_pFormatCtx->streams[i];
            _pInCtx->m_nVideoIndex = i;

            if (fill_stream_info(_pInCtx->m_pVideoStream, &_pInCtx->m_pVideoCodec, &_pInCtx->m_pVideoCodecCtx)) {return -1;}
        } else if (_pInCtx->m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            _pInCtx->m_pAudioStream = _pInCtx->m_pFormatCtx->streams[i];
            _pInCtx->m_nAudioIndex = i;

            if (fill_stream_info(_pInCtx->m_pAudioStream, &_pInCtx->m_pAudioCodec, &_pInCtx->m_pAudioCodecCtx)) {return -1;}
        } else {
            av_log(NULL, AV_LOG_ERROR, "skipping streams other than audio and video");
        }
    }
}

int init_decoder_stream_ctx(StreamContext** _ppInCtx, const AVFormatContext* _pDecodeCtx)
{
    (*_ppInCtx)->m_pVideoCodec = _pDecodeCtx->video_codec;
    (*_ppInCtx)->m_pAudioCodec = _pDecodeCtx->audio_codec;
    get_stream_refs(*_ppInCtx);
}

int init_default_params(StreamingParams** _ppParams)
{
    (*_ppParams)->m_nRemuxAudio = 0;
    (*_ppParams)->m_nRemuxVideo = 0;
    (*_ppParams)->m_pOutExt = "ts";
    (*_ppParams)->m_nVideoID = AV_CODEC_ID_HEVC;
    (*_ppParams)->m_pCodecPrivKey = "x265-params";
    (*_ppParams)->m_pCodecPrivVal = "keyint=60:min-keyint=60:scenecut=0";
    (*_ppParams)->m_nAudioID = AV_CODEC_ID_AAC;
}

int init_context(TranscodeContext* _pT, StreamingParams* _pParams)
{
    avformat_alloc_output_context2(&_pT->m_pEncoderCtx->m_pFormatCtx, NULL, NULL, _pT->m_pOutParams->m_pFQN);
    AVRational framerate = av_guess_frame_rate(_pT->m_pDecoderCtx->m_pFormatCtx, _pT->m_pDecoderCtx->m_pVideoStream, NULL);\
    _pT->m_nSrcFrameRate = framerate;
    init_video_encoder(_pT->m_pEncoderCtx, _pT->m_pDecoderCtx->m_pVideoCodecCtx, framerate, *_pParams);
    init_audio_encoder(_pT->m_pEncoderCtx, _pT->m_pDecoderCtx->m_pAudioCodecCtx->sample_rate, *_pParams);

    if (_pT->m_pEncoderCtx->m_pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER)
        _pT->m_pEncoderCtx->m_pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    if (!(_pT->m_pEncoderCtx->m_pFormatCtx->oformat->flags & AVFMT_NOFILE))
    {
        if (avio_open(&_pT->m_pEncoderCtx->m_pFormatCtx->pb, _pT->m_pOutParams->m_pFQP, AVIO_FLAG_WRITE) < 0) {
            av_log(NULL, AV_LOG_ERROR, "could not open the output file");
            return -1;
        }
    }

    if (_pParams->m_pMuxerOptKey && _pParams->m_pMuxerOptVal)
    {
        av_dict_set(&_pT->m_pMuxer_Opts, _pParams->m_pMuxerOptKey, _pParams->m_pMuxerOptVal, 0);
    }
}

int encode_video(StreamContext* _pDecodeCtx, StreamContext* _pEncodeCtx, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_)
{
    if (_pInputFrame) _pInputFrame->pict_type = AV_PICTURE_TYPE_NONE;

    int response = avcodec_send_frame(_pEncodeCtx->m_pVideoCodecCtx, _pInputFrame);

    while (response >= 0)
    {
        response = avcodec_receive_packet(_pEncodeCtx->m_pVideoCodecCtx, *_ppOutPkt_);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            break;
        else if (response < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Error while receiving packet from _pEncodeCtx: %s", av_err2str(response));
            return -1;
        }

        (*_ppOutPkt_)->stream_index = _pDecodeCtx->m_nVideoIndex;
        (*_ppOutPkt_)->duration = _pEncodeCtx->m_pVideoStream->time_base.den / _pEncodeCtx->m_pVideoStream->time_base.num / _pDecodeCtx->m_pVideoStream->avg_frame_rate.num * _pDecodeCtx->m_pVideoStream->avg_frame_rate.den;

        av_packet_rescale_ts(*_ppOutPkt_, _pDecodeCtx->m_pVideoStream->time_base, _pEncodeCtx->m_pVideoStream->time_base);
    }
    return 0;
}

int encode_audio(StreamContext* _pDecodeCtx, StreamContext* _pEncodeCtx, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_)
{
    int response = avcodec_send_frame(_pEncodeCtx->m_pAudioCodecCtx, _pInputFrame);

    while (response >= 0)
    {
        response = avcodec_receive_packet(_pEncodeCtx->m_pAudioCodecCtx, *_ppOutPkt_);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            break;
        else if (response < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Error while receiving packet from _pEncodeCtx: %s", av_err2str(response));
            return -1;
        }

        (*_ppOutPkt_)->stream_index = _pDecodeCtx->m_nAudioIndex;

        av_packet_rescale_ts(*_ppOutPkt_, _pDecodeCtx->m_pAudioStream->time_base, _pEncodeCtx->m_pAudioStream->time_base);
    }
    return 0;
}

int remux(TranscodeContext* _pT, AVPacket** _ppInputPkt, const char _nVideo, AVPacket** _ppOutputPkt_)
{
    AVRational src_timebase, dst_timebase;
    if (_nVideo)
    {
        src_timebase = _pT->m_pDecoderCtx->m_pVideoStream->time_base;
        dst_timebase = _pT->m_pEncoderCtx->m_pVideoStream->time_base;
    }
    else
    {
        src_timebase = _pT->m_pDecoderCtx->m_pAudioStream->time_base;
        dst_timebase = _pT->m_pEncoderCtx->m_pAudioStream->time_base;
    }

    av_packet_rescale_ts(*_ppInputPkt, src_timebase, dst_timebase);
    av_packet_move_ref(*_ppOutputPkt_, *_ppInputPkt);

    return 0;
}

//////



TranscodeContext* alloc_transcoder()
{
    TranscodeContext* pT = malloc(sizeof(TranscodeContext));
    pT->m_pEncoderCtx = malloc(sizeof(StreamContext));
    pT->m_pDecoderCtx = malloc(sizeof(StreamContext));
    pT->m_pOutParams = malloc(sizeof(StreamingParams));
    return pT;
}

void free_transcoder(TranscodeContext* _pT)
{
    if (_pT->m_pMuxer_Opts != NULL)
    {
        av_dict_free(&_pT->m_pMuxer_Opts);
        _pT->m_pMuxer_Opts = NULL;
    }

    avformat_free_context(_pT->m_pEncoderCtx->m_pFormatCtx);
    _pT->m_pEncoderCtx->m_pFormatCtx = NULL;

    avcodec_free_context(&_pT->m_pEncoderCtx->m_pVideoCodecCtx);
    _pT->m_pEncoderCtx->m_pVideoCodecCtx = NULL;
    avcodec_free_context(&_pT->m_pEncoderCtx->m_pAudioCodecCtx);
    _pT->m_pEncoderCtx->m_pAudioCodecCtx = NULL;

    free(_pT->m_pEncoderCtx);
    _pT->m_pEncoderCtx = NULL;

    free(_pT->m_pDecoderCtx);
    _pT->m_pDecoderCtx = NULL;

    free(_pT);
    _pT = NULL;
}

int init_transcoder(TranscodeContext* _pT, StreamingParams* _pParams, AVFormatContext* _pDecodeCtx, const char* _pFN)
{
    if (_pParams)
        _pT->m_pOutParams = _pParams;
    else
        init_default_params(&_pT->m_pOutParams);

    _pT->m_pOutParams->m_pFN = _pFN;
    init_output_filename(&_pT);
    init_decoder_stream_ctx(&_pT->m_pDecoderCtx, _pDecodeCtx);
    init_context(_pT, _pT->m_pOutParams);

    if (_pT->m_pDecoderCtx->m_pVideoCodec->id == _pT->m_pOutParams->m_nVideoID)
        _pT->m_pOutParams->m_nRemuxVideo = 1;

    if (_pT->m_pDecoderCtx->m_pAudioCodec->id == _pT->m_pOutParams->m_nRemuxAudio)
        _pT->m_pOutParams->m_nRemuxAudio = 1;
}

int transcode_audio(TranscodeContext* _pT, AVPacket* _pInputPkt, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_)
{
    int response = avcodec_send_packet(_pT->m_pDecoderCtx->m_pAudioCodecCtx, _pInputPkt);
    if (response < 0) {av_log(NULL, AV_LOG_ERROR, "Error while sending packet to _pDecodeCtx: %s", av_err2str(response)); return response;}

    if (!*_ppOutPkt_)
        *_ppOutPkt_ = av_packet_alloc();

    if (!*_ppOutPkt_)
        return -1;
    if (!_pT->m_pOutParams->m_nRemuxVideo)
    {
        while (response >= 0) {
            response = avcodec_receive_frame(_pT->m_pDecoderCtx->m_pAudioCodecCtx, _pInputFrame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                break;
            else if (response < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error while receiving frame from _pDecodeCtx: %s", av_err2str(response));
                return response;
            }

            if (response >= 0) {
                if (encode_audio(_pT->m_pDecoderCtx, _pT->m_pEncoderCtx, _pInputFrame, _ppOutPkt_)) return -1;
            }
        }
    }
    else
    {
        remux(_pT, &_pInputPkt, 0, _ppOutPkt_);
    }

    return 0;
}

int transcode_video(TranscodeContext* _pT, AVPacket* _pInputPkt, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_)
{
    int response = avcodec_send_packet(_pT->m_pDecoderCtx->m_pVideoCodecCtx, _pInputPkt);
    if (response < 0) {av_log(NULL, AV_LOG_ERROR, "Error while sending packet to _pDecodeCtx: %s", av_err2str(response)); return response;}

    if (!*_ppOutPkt_)
        *_ppOutPkt_ = av_packet_alloc();

    if (!*_ppOutPkt_)
        return -1;

    if (!_pT->m_pOutParams->m_nRemuxVideo)
    {
        while (response >= 0) {
            response = avcodec_receive_frame(_pT->m_pDecoderCtx->m_pVideoCodecCtx, _pInputFrame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                break;
            else if (response < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error while receiving frame from _pDecodeCtx: %s", av_err2str(response));
                return response;
            }

            if (response >= 0) {
                if (encode_video(_pT->m_pDecoderCtx, _pT->m_pEncoderCtx, _pInputFrame, _ppOutPkt_)) return -1;
            }
        }
    }
    else
    {
        remux(_pT, &_pInputPkt, 1, _ppOutPkt_);
    }

    return 0;
}

int rollover_split(TranscodeContext* _pT)
{
    if (avio_close(_pT->m_pEncoderCtx->m_pFormatCtx->pb) < 0) {
        av_log(NULL, AV_LOG_ERROR, "could not close the output file");
        return -1;
    }

    init_output_filename(&_pT);

    if (avio_open(&_pT->m_pEncoderCtx->m_pFormatCtx->pb, _pT->m_pOutParams->m_pFQP, AVIO_FLAG_WRITE) < 0) {
        av_log(NULL, AV_LOG_ERROR, "could not open the output file");
        return -1;
    }

    return 0;
}