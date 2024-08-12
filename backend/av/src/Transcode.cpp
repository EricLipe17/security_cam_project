#include "Transcode.hpp"

#include <libavutil/opt.h>
#include <libavutil/timestamp.h>

#include <string>

#include "AVUtil.hpp"

#define STORAGE_DIR "/tmp/singularity_output/"

////// Private methods
int TranscodeContext::init_output_filename() {
    if (!m_pOutParams->m_pFQP) free(m_pOutParams->m_pFQP);
    get_next_filename(m_pOutParams->m_pFN, m_pOutParams->m_pOutExt, &m_pOutParams->m_pFQN);

    const size_t nSizeDir = strlen(STORAGE_DIR);
    const size_t nSizeFQP = strlen(m_pOutParams->m_pFQN);

    m_pOutParams->m_pFQP = new char(sizeof(char) * (nSizeDir + nSizeFQP));
    stpncpy(m_pOutParams->m_pFQP, STORAGE_DIR, nSizeDir);
    stpncpy(m_pOutParams->m_pFQP + nSizeDir, m_pOutParams->m_pFQN, nSizeFQP);
    return 0;
}

int open_media(const char* _pUrl, AVFormatContext** _ppFmtCtx) {
    *_ppFmtCtx = avformat_alloc_context();
    if (!*_ppFmtCtx) {
        av_log(nullptr, AV_LOG_ERROR, "failed to alloc memory for format");
        return -1;
    }

    if (avformat_open_input(_ppFmtCtx, _pUrl, nullptr, nullptr) != 0) {
        av_log(nullptr, AV_LOG_ERROR, "failed to open input file %s", _pUrl);
        return -1;
    }

    if (avformat_find_stream_info(*_ppFmtCtx, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "failed to get stream info");
        return -1;
    }
    return 0;
}

int TranscodeContext::init_video_encoder(StreamContext* _pCtx, AVCodecContext* _pDecoderCodecCtx,
                                         AVRational framerate, StreamingParams params) {
    _pCtx->m_pVideoStream = avformat_new_stream(_pCtx->m_pFormatCtx, nullptr);

    _pCtx->m_pVideoCodec = avcodec_find_encoder(params.m_nVideoID);
    if (!_pCtx->m_pVideoCodec) {
        av_log(nullptr, AV_LOG_ERROR, "could not find the proper codec");
        return -1;
    }

    _pCtx->m_pVideoCodecCtx = avcodec_alloc_context3(_pCtx->m_pVideoCodec);
    if (!_pCtx->m_pVideoCodecCtx) {
        av_log(nullptr, AV_LOG_ERROR, "could not allocated memory for codec context");
        return -1;
    }

    av_opt_set(_pCtx->m_pVideoCodecCtx->priv_data, "preset", "fast", 0);
    if (params.m_pCodecPrivKey && params.m_pCodecPrivVal)
        av_opt_set(_pCtx->m_pVideoCodecCtx->priv_data, params.m_pCodecPrivKey,
                   params.m_pCodecPrivVal, 0);

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

    if (avcodec_open2(_pCtx->m_pVideoCodecCtx, _pCtx->m_pVideoCodec, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "could not open the codec");
        return -1;
    }
    avcodec_parameters_from_context(_pCtx->m_pVideoStream->codecpar, _pCtx->m_pVideoCodecCtx);
    return 0;
}

int TranscodeContext::init_audio_encoder(StreamContext* _pCtx, int sample_rate,
                                         StreamingParams params) {
    _pCtx->m_pAudioStream = avformat_new_stream(_pCtx->m_pFormatCtx, nullptr);

    _pCtx->m_pAudioCodec = avcodec_find_encoder(params.m_nAudioID);
    if (!_pCtx->m_pAudioCodec) {
        av_log(nullptr, AV_LOG_ERROR, "could not find the proper codec");
        return -1;
    }

    _pCtx->m_pAudioCodecCtx = avcodec_alloc_context3(_pCtx->m_pAudioCodec);
    if (!_pCtx->m_pAudioCodecCtx) {
        av_log(nullptr, AV_LOG_ERROR, "could not allocate memory for codec context");
        return -1;
    }

    int OUTPUT_CHANNELS = 2;
    int OUTPUT_BIT_RATE = 196000;
    _pCtx->m_pAudioCodecCtx->ch_layout.nb_channels = OUTPUT_CHANNELS;
    av_channel_layout_default(&_pCtx->m_pAudioCodecCtx->ch_layout, OUTPUT_CHANNELS);
    _pCtx->m_pAudioCodecCtx->sample_rate = sample_rate;
    _pCtx->m_pAudioCodecCtx->sample_fmt = _pCtx->m_pAudioCodec->sample_fmts[0];
    _pCtx->m_pAudioCodecCtx->bit_rate = OUTPUT_BIT_RATE;
    _pCtx->m_pAudioCodecCtx->time_base = (AVRational){1, sample_rate};

    _pCtx->m_pAudioCodecCtx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    _pCtx->m_pAudioStream->time_base = _pCtx->m_pAudioCodecCtx->time_base;

    if (avcodec_open2(_pCtx->m_pAudioCodecCtx, _pCtx->m_pAudioCodec, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "could not open the codec");
        return -1;
    }
    avcodec_parameters_from_context(_pCtx->m_pAudioStream->codecpar, _pCtx->m_pAudioCodecCtx);
    return 0;
}

int TranscodeContext::fill_stream_info(AVStream* _pStream, const AVCodec** _ppCodec,
                                       AVCodecContext** _ppCodecCtx) {
    *_ppCodec = avcodec_find_decoder(_pStream->codecpar->codec_id);
    if (!*_ppCodec) {
        av_log(nullptr, AV_LOG_ERROR, "failed to find the codec");
        return -1;
    }

    *_ppCodecCtx = avcodec_alloc_context3(*_ppCodec);
    if (!*_ppCodecCtx) {
        av_log(nullptr, AV_LOG_ERROR, "failed to allocate memory for codec context");
        return -1;
    }

    if (avcodec_parameters_to_context(*_ppCodecCtx, _pStream->codecpar) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "failed to fill codec context");
        return -1;
    }

    if (avcodec_open2(*_ppCodecCtx, *_ppCodec, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "failed to open codec");
        return -1;
    }
    return 0;
}

int TranscodeContext::get_stream_refs(StreamContext* _pInCtx) {
    for (int i = 0; i < _pInCtx->m_pFormatCtx->nb_streams; i++) {
        if (_pInCtx->m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            _pInCtx->m_pVideoStream = _pInCtx->m_pFormatCtx->streams[i];
            _pInCtx->m_nVideoIndex = i;

            if (fill_stream_info(_pInCtx->m_pVideoStream, &_pInCtx->m_pVideoCodec,
                                 &_pInCtx->m_pVideoCodecCtx)) {
                return -1;
            }
        } else if (_pInCtx->m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            _pInCtx->m_pAudioStream = _pInCtx->m_pFormatCtx->streams[i];
            _pInCtx->m_nAudioIndex = i;

            if (fill_stream_info(_pInCtx->m_pAudioStream, &_pInCtx->m_pAudioCodec,
                                 &_pInCtx->m_pAudioCodecCtx)) {
                return -1;
            }
        } else {
            av_log(nullptr, AV_LOG_WARNING, "skipping streams other than audio and video");
        }
    }
    return 0;
}

int TranscodeContext::init_decoder_stream_ctx(StreamContext** _ppInCtx,
                                              const AVFormatContext* _pDecodeCtx) {
    (*_ppInCtx)->m_pVideoCodec = _pDecodeCtx->video_codec;
    (*_ppInCtx)->m_pAudioCodec = _pDecodeCtx->audio_codec;
    return get_stream_refs(*_ppInCtx);
}

int TranscodeContext::init_default_params(StreamingParams** _ppParams) {
    (*_ppParams)->m_nRemuxAudio = 0;
    (*_ppParams)->m_nRemuxVideo = 0;
    (*_ppParams)->m_pOutExt = (char*)("ts");
    (*_ppParams)->m_nVideoID = AV_CODEC_ID_HEVC;
    (*_ppParams)->m_pCodecPrivKey = (char*)("x265-params");
    (*_ppParams)->m_pCodecPrivVal = (char*)("keyint=60:min-keyint=60:scenecut=0");
    (*_ppParams)->m_nAudioID = AV_CODEC_ID_AAC;
    return 0;
}

int TranscodeContext::init_context(StreamingParams* _pParams) {
    avformat_alloc_output_context2(&m_pEncoderCtx->m_pFormatCtx, nullptr, nullptr,
                                   m_pOutParams->m_pFQN);
    AVRational framerate =
        av_guess_frame_rate(m_pDecoderCtx->m_pFormatCtx, m_pDecoderCtx->m_pVideoStream, nullptr);
    m_nSrcFrameRate = framerate;
    init_video_encoder(m_pEncoderCtx, m_pDecoderCtx->m_pVideoCodecCtx, framerate, *_pParams);
    init_audio_encoder(m_pEncoderCtx, m_pDecoderCtx->m_pAudioCodecCtx->sample_rate, *_pParams);

    if (m_pEncoderCtx->m_pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER)
        m_pEncoderCtx->m_pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    if (!(m_pEncoderCtx->m_pFormatCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&m_pEncoderCtx->m_pFormatCtx->pb, m_pOutParams->m_pFQP, AVIO_FLAG_WRITE) <
            0) {
            av_log(nullptr, AV_LOG_ERROR, "could not open the output file");
            return -1;
        }
    }

    if (_pParams->m_pMuxerOptKey && _pParams->m_pMuxerOptVal) {
        return av_dict_set(&m_pMuxer_Opts, _pParams->m_pMuxerOptKey, _pParams->m_pMuxerOptVal, 0);
    }
    return 0;
}

int TranscodeContext::encode_video(StreamContext* _pDecodeCtx, StreamContext* _pEncodeCtx,
                                   AVFrame* _pInputFrame, AVPacket** _ppOutPkt_) {
    if (_pInputFrame) _pInputFrame->pict_type = AV_PICTURE_TYPE_NONE;

    int response = avcodec_send_frame(_pEncodeCtx->m_pVideoCodecCtx, _pInputFrame);

    while (response >= 0) {
        response = avcodec_receive_packet(_pEncodeCtx->m_pVideoCodecCtx, *_ppOutPkt_);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            break;
        else if (response < 0) {
            std::string szResponse = get_error_msg(response);
            av_log(nullptr, AV_LOG_ERROR, "Error while receiving packet from _pEncodeCtx: %s",
                   szResponse.c_str());
            return -1;
        }

        (*_ppOutPkt_)->stream_index = _pDecodeCtx->m_nVideoIndex;
        (*_ppOutPkt_)->duration = _pEncodeCtx->m_pVideoStream->time_base.den /
                                  _pEncodeCtx->m_pVideoStream->time_base.num /
                                  _pDecodeCtx->m_pVideoStream->avg_frame_rate.num *
                                  _pDecodeCtx->m_pVideoStream->avg_frame_rate.den;

        av_packet_rescale_ts(*_ppOutPkt_, _pDecodeCtx->m_pVideoStream->time_base,
                             _pEncodeCtx->m_pVideoStream->time_base);
    }
    return 0;
}

int TranscodeContext::encode_audio(StreamContext* _pDecodeCtx, StreamContext* _pEncodeCtx,
                                   AVFrame* _pInputFrame, AVPacket** _ppOutPkt_) {
    int response = avcodec_send_frame(_pEncodeCtx->m_pAudioCodecCtx, _pInputFrame);

    while (response >= 0) {
        response = avcodec_receive_packet(_pEncodeCtx->m_pAudioCodecCtx, *_ppOutPkt_);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            break;
        else if (response < 0) {
            std::string szResponse = get_error_msg(response);
            av_log(nullptr, AV_LOG_ERROR, "Error while receiving packet from _pEncodeCtx: %s",
                   szResponse.c_str());
            return -1;
        }

        (*_ppOutPkt_)->stream_index = _pDecodeCtx->m_nAudioIndex;

        av_packet_rescale_ts(*_ppOutPkt_, _pDecodeCtx->m_pAudioStream->time_base,
                             _pEncodeCtx->m_pAudioStream->time_base);
    }
    return 0;
}

int TranscodeContext::remux(AVPacket** _ppInputPkt, const char _nVideo, AVPacket** _ppOutputPkt_) {
    AVRational src_timebase, dst_timebase;
    if (_nVideo) {
        src_timebase = m_pDecoderCtx->m_pVideoStream->time_base;
        dst_timebase = m_pEncoderCtx->m_pVideoStream->time_base;
    } else {
        src_timebase = m_pDecoderCtx->m_pAudioStream->time_base;
        dst_timebase = m_pEncoderCtx->m_pAudioStream->time_base;
    }

    av_packet_rescale_ts(*_ppInputPkt, src_timebase, dst_timebase);
    av_packet_move_ref(*_ppOutputPkt_, *_ppInputPkt);

    return 0;
}

//////

TranscodeContext::~TranscodeContext() {
    if (m_pMuxer_Opts != nullptr) {
        av_dict_free(&m_pMuxer_Opts);
        m_pMuxer_Opts = nullptr;
    }

    avformat_free_context(m_pEncoderCtx->m_pFormatCtx);
    m_pEncoderCtx->m_pFormatCtx = nullptr;

    avcodec_free_context(&m_pEncoderCtx->m_pVideoCodecCtx);
    m_pEncoderCtx->m_pVideoCodecCtx = nullptr;
    avcodec_free_context(&m_pEncoderCtx->m_pAudioCodecCtx);
    m_pEncoderCtx->m_pAudioCodecCtx = nullptr;

    free(m_pEncoderCtx);
    m_pEncoderCtx = nullptr;

    free(m_pDecoderCtx);
    m_pDecoderCtx = nullptr;
}

TranscodeContext::TranscodeContext(StreamingParams* _pParams, AVFormatContext* _pDecodeCtx,
                                   char* _pFN) {
    if (_pParams)
        m_pOutParams = _pParams;
    else
        init_default_params(&m_pOutParams);

    m_pOutParams->m_pFN = _pFN;
    init_output_filename();
    init_decoder_stream_ctx(&m_pDecoderCtx, _pDecodeCtx);
    init_context(m_pOutParams);

    if (m_pDecoderCtx->m_pVideoCodec->id == m_pOutParams->m_nVideoID)
        m_pOutParams->m_nRemuxVideo = 1;

    if (m_pDecoderCtx->m_pAudioCodec->id == m_pOutParams->m_nRemuxAudio)
        m_pOutParams->m_nRemuxAudio = 1;
}

int TranscodeContext::transcode_audio(AVPacket* _pInputPkt, AVFrame* _pInputFrame,
                                      AVPacket** _ppOutPkt_) {
    int response = avcodec_send_packet(m_pDecoderCtx->m_pAudioCodecCtx, _pInputPkt);
    if (response < 0) {
        std::string szResponse = get_error_msg(response);
        av_log(nullptr, AV_LOG_ERROR, "Error while sending packet to _pDecodeCtx: %s",
               szResponse.c_str());
        return response;
    }

    if (!*_ppOutPkt_) *_ppOutPkt_ = av_packet_alloc();

    if (!*_ppOutPkt_) return -1;
    if (!m_pOutParams->m_nRemuxVideo) {
        while (response >= 0) {
            response = avcodec_receive_frame(m_pDecoderCtx->m_pAudioCodecCtx, _pInputFrame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                break;
            else if (response < 0) {
                std::string szResponse = get_error_msg(response);
                av_log(nullptr, AV_LOG_ERROR, "Error while receiving frame from _pDecodeCtx: %s",
                       szResponse.c_str());
                return response;
            }

            if (response >= 0) {
                if (encode_audio(m_pDecoderCtx, m_pEncoderCtx, _pInputFrame, _ppOutPkt_)) return -1;
            }
        }
    } else {
        remux(&_pInputPkt, 0, _ppOutPkt_);
    }

    return 0;
}

int TranscodeContext::transcode_video(AVPacket* _pInputPkt, AVFrame* _pInputFrame,
                                      AVPacket** _ppOutPkt_) {
    int response = avcodec_send_packet(m_pDecoderCtx->m_pVideoCodecCtx, _pInputPkt);
    if (response < 0) {
        std::string szResponse = get_error_msg(response);
        av_log(nullptr, AV_LOG_ERROR, "Error while sending packet to _pDecodeCtx: %s",
               szResponse.c_str());
        return response;
    }

    if (!*_ppOutPkt_) *_ppOutPkt_ = av_packet_alloc();

    if (!*_ppOutPkt_) return -1;

    if (!m_pOutParams->m_nRemuxVideo) {
        while (response >= 0) {
            response = avcodec_receive_frame(m_pDecoderCtx->m_pVideoCodecCtx, _pInputFrame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                break;
            else if (response < 0) {
                std::string szResponse = get_error_msg(response);
                av_log(nullptr, AV_LOG_ERROR, "Error while receiving frame from _pDecodeCtx: %s",
                       szResponse.c_str());
                return response;
            }

            if (response >= 0) {
                if (encode_video(m_pDecoderCtx, m_pEncoderCtx, _pInputFrame, _ppOutPkt_)) return -1;
            }
        }
    } else {
        remux(&_pInputPkt, 1, _ppOutPkt_);
    }

    return 0;
}

int TranscodeContext::rollover_split() {
    if (avio_close(m_pEncoderCtx->m_pFormatCtx->pb) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "could not close the output file");
        return -1;
    }

    init_output_filename();

    if (avio_open(&m_pEncoderCtx->m_pFormatCtx->pb, m_pOutParams->m_pFQP, AVIO_FLAG_WRITE) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "could not open the output file");
        return -1;
    }

    return 0;
}

AVFormatContext* TranscodeContext::GetDecoderFmtCtx() { return m_pDecoderCtx->m_pFormatCtx; }

AVRational TranscodeContext::GetSrcFrameRate() { return m_nSrcFrameRate; }

AVDictionary** TranscodeContext::GetMuxerOpts() { return &m_pMuxer_Opts; }