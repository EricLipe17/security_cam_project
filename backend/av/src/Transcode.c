#include "Transcode.h"

StreamContext* alloc_stream_context()
{
    StreamContext* pS = malloc(sizeof(StreamContext));
    pS->m_pFmtCtx = avformat_alloc_context();
    pS->m_pPkt = av_packet_alloc();
    pS->m_pFrame = av_frame_alloc();
    return pS;
}

TranscodeContext* alloc_transcoder()
{
    TranscodeContext* pT = malloc(sizeof(TranscodeContext));
    pT->m_pDecodeCtx = alloc_stream_context();
    pT->m_pEncodeCtx = alloc_stream_context();
    return pT;
}

void free_transcoder(TranscodeContext* _pT)
{
    // Free CodecContexts for all
    for (int i = 0; i < _pT->m_pDecodeCtx->m_pFmtCtx->nb_streams; ++i) {
        avcodec_free_context(&_pT->m_pDecodeCtx->m_pArrCodecCtx[i]);
        avcodec_free_context(&_pT->m_pEncodeCtx->m_pArrCodecCtx[i]);
        _pT->m_pDecodeCtx->m_pArrCodecCtx[i] = NULL;
        _pT->m_pEncodeCtx->m_pArrCodecCtx[i] = NULL;
    }
    av_free(_pT->m_pDecodeCtx->m_pArrCodecCtx);
    _pT->m_pDecodeCtx->m_pArrCodecCtx = NULL;
    av_free(_pT->m_pEncodeCtx->m_pArrCodecCtx);
    _pT->m_pEncodeCtx->m_pArrCodecCtx = NULL;

    // Free input resources
    avformat_free_context(_pT->m_pDecodeCtx->m_pFmtCtx);
    _pT->m_pDecodeCtx->m_pFmtCtx = NULL;

    av_frame_free(&_pT->m_pDecodeCtx->m_pFrame);
    _pT->m_pDecodeCtx->m_pFrame = NULL;

    av_packet_free(&_pT->m_pDecodeCtx->m_pPkt);
    _pT->m_pDecodeCtx->m_pPkt = NULL;

    // Free output resources
    avformat_free_context(_pT->m_pEncodeCtx->m_pFmtCtx);
    _pT->m_pEncodeCtx->m_pFmtCtx = NULL;

    av_frame_free(&_pT->m_pEncodeCtx->m_pFrame);
    _pT->m_pEncodeCtx->m_pFrame = NULL;

    av_packet_free(&_pT->m_pEncodeCtx->m_pPkt);
    _pT->m_pEncodeCtx->m_pPkt = NULL;

    // Free stream contexts
    free(_pT->m_pDecodeCtx);
    _pT->m_pDecodeCtx = NULL;
    free(_pT->m_pEncodeCtx);
    _pT->m_pEncodeCtx = NULL;

    // Free transcoder instance
    free(_pT);
    _pT = NULL;
}

int open_input(StreamContext* _pInStreamCtx)
{
    int ret;
    unsigned int i;

    if ((ret = avformat_open_input(&_pInStreamCtx->m_pFmtCtx, _pInStreamCtx->m_pParams->m_pFQN, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(_pInStreamCtx->m_pFmtCtx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    _pInStreamCtx->m_pArrCodecCtx = av_calloc(_pInStreamCtx->m_pFmtCtx->nb_streams, sizeof(AVCodecContext));
    if (!_pInStreamCtx->m_pArrCodecCtx)
        return AVERROR(ENOMEM);

    for (i = 0; i < _pInStreamCtx->m_pFmtCtx->nb_streams; i++) {
        AVStream *stream = _pInStreamCtx->m_pFmtCtx->streams[i];
        const AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        AVCodecContext *codec_ctx;
        if (!dec) {
            av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
            return AVERROR_DECODER_NOT_FOUND;
        }
        codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
            return AVERROR(ENOMEM);
        }
        ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
                                       "for stream #%u\n", i);
            return ret;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
                codec_ctx->framerate = av_guess_frame_rate(_pInStreamCtx->m_pFmtCtx, stream, NULL);
            /* Open decoder */
            ret = avcodec_open2(codec_ctx, dec, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
                return ret;
            }
        }
        _pInStreamCtx->m_pArrCodecCtx[i] = codec_ctx;

        _pInStreamCtx->m_pFrame = av_frame_alloc();
        if (!_pInStreamCtx->m_pFrame)
            return AVERROR(ENOMEM);
    }

    av_dump_format(_pInStreamCtx->m_pFmtCtx, 0, _pInStreamCtx->m_pParams->m_pFQN, 0);
    return 0;
}

int open_output(StreamContext* _pOutStreamCtx, StreamContext* _pInStreamCtx)
{
    AVStream *out_stream;
    AVStream *in_stream;
    AVCodecContext *dec_ctx, *enc_ctx;
    const AVCodec *encoder;
    int ret;
    unsigned int i;

    avformat_alloc_output_context2(&_pOutStreamCtx->m_pFmtCtx, NULL, NULL, _pOutStreamCtx->m_pParams->m_pFQN);
    if (!_pOutStreamCtx->m_pFmtCtx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return AVERROR_UNKNOWN;
    }

    _pOutStreamCtx->m_pArrCodecCtx = av_calloc(_pInStreamCtx->m_pFmtCtx->nb_streams, sizeof(AVCodecContext));
    if (!_pOutStreamCtx->m_pArrCodecCtx)
        return AVERROR(ENOMEM);

    for (i = 0; i < _pInStreamCtx->m_pFmtCtx->nb_streams; i++) {
        out_stream = avformat_new_stream(_pOutStreamCtx->m_pFmtCtx, NULL);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            return AVERROR_UNKNOWN;
        }

        in_stream = _pInStreamCtx->m_pFmtCtx->streams[i];
        dec_ctx = _pInStreamCtx->m_pArrCodecCtx[i];

        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* in this example, we choose transcoding to same codec */
            encoder = avcodec_find_encoder(dec_ctx->codec_id);
            if (!encoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                return AVERROR_INVALIDDATA;
            }
            enc_ctx = avcodec_alloc_context3(encoder);
            if (!enc_ctx) {
                av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
                return AVERROR(ENOMEM);
            }

            /* In this example, we transcode to same properties (picture size,
             * sample rate etc.). These properties can be changed for output
             * streams easily using filters */
            if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                enc_ctx->height = dec_ctx->height;
                enc_ctx->width = dec_ctx->width;
                enc_ctx->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
                /* take first format from list of supported formats */
                if (encoder->pix_fmts)
                    enc_ctx->pix_fmt = encoder->pix_fmts[0];
                else
                    enc_ctx->pix_fmt = dec_ctx->pix_fmt;
                /* video time_base can be set to whatever is handy and supported by encoder */
                enc_ctx->time_base = av_inv_q(dec_ctx->framerate);
            } else {
                enc_ctx->sample_rate = dec_ctx->sample_rate;
                ret = av_channel_layout_copy(&enc_ctx->ch_layout, &dec_ctx->ch_layout);
                if (ret < 0)
                    return ret;
                /* take first format from list of supported formats */
                enc_ctx->sample_fmt = encoder->sample_fmts[0];
                enc_ctx->time_base = (AVRational){1, enc_ctx->sample_rate};
            }

            if (_pOutStreamCtx->m_pFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
                enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            /* Third parameter can be used to pass settings to encoder */
            ret = avcodec_open2(enc_ctx, encoder, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", i);
                return ret;
            }
            ret = avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream #%u\n", i);
                return ret;
            }

            out_stream->time_base = enc_ctx->time_base;
            _pOutStreamCtx->m_pArrCodecCtx[i] = enc_ctx;
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
            return AVERROR_INVALIDDATA;
        } else {
            /* if this stream must be remuxed */
            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", i);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
        }

    }
    av_dump_format(_pOutStreamCtx->m_pFmtCtx, 0, _pOutStreamCtx->m_pParams->m_pFQN, 1);

    if (!(_pOutStreamCtx->m_pFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&_pOutStreamCtx->m_pFmtCtx->pb, _pOutStreamCtx->m_pParams->m_pFQN, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", _pOutStreamCtx->m_pParams->m_pFQN);
            return ret;
        }
    }

    /* init muxer, write output file header */
    ret = avformat_write_header(_pOutStreamCtx->m_pFmtCtx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    return 0;
}

void init_transcoder(TranscodeContext* _pT, StreamParams* _pIn, StreamParams* _pOut)
{
    _pT->m_pDecodeCtx->m_pParams = _pIn;
    _pT->m_pEncodeCtx->m_pParams = _pOut;

    open_input(_pT->m_pDecodeCtx);
    open_output(_pT->m_pEncodeCtx, _pT->m_pDecodeCtx);
}

int receive_frame(StreamContext* _pS, int* _nStreamIndex_)
{
    int ret;

    // Receive frame from decoder
    av_packet_unref(_pS->m_pPkt);
    av_read_frame(_pS->m_pFmtCtx, _pS->m_pPkt);
    *_nStreamIndex_ = _pS->m_pPkt->stream_index;
    av_log(NULL, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n", *_nStreamIndex_);
    av_packet_rescale_ts(_pS->m_pPkt,
                         _pS->m_pFmtCtx->streams[*_nStreamIndex_]->time_base,
                         _pS->m_pArrCodecCtx[*_nStreamIndex_]->time_base);
    ret = avcodec_send_packet(_pS->m_pArrCodecCtx[*_nStreamIndex_], _pS->m_pPkt);

    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
        return ret;
    }

    return ret;
}

int internal_write_frame(StreamContext* pStreamCtx, AVCodecContext* _pEncoder,
                         AVFrame* _pDecodedFrame, int _nStreamIndex)
{
    int ret;

    av_packet_unref(pStreamCtx->m_pPkt);

    ret = avcodec_send_frame(_pEncoder, _pDecodedFrame);

    while (ret >= 0) {
        ret = avcodec_receive_packet(_pEncoder, pStreamCtx->m_pPkt);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return 0;

        /* prepare packet for muxing */
        pStreamCtx->m_pPkt->stream_index = _nStreamIndex;
        av_packet_rescale_ts(pStreamCtx->m_pPkt,
                             _pEncoder->time_base,
                             pStreamCtx->m_pFmtCtx->streams[_nStreamIndex]->time_base);

        av_log(NULL, AV_LOG_DEBUG, "Muxing frame\n");
        /* mux encoded frame */
        ret = av_interleaved_write_frame(pStreamCtx->m_pFmtCtx, pStreamCtx->m_pPkt);
    }

    return ret;
}

int flush_encoder(TranscodeContext* _pT, int _nSteamIndex)
{
    if (!(_pT->m_pEncodeCtx->m_pArrCodecCtx[_nSteamIndex]->codec->capabilities &
          AV_CODEC_CAP_DELAY))
        return 0;

    av_log(NULL, AV_LOG_INFO, "Flushing stream #%u encoder\n", _nSteamIndex);

    AVCodecContext* pEncoder= _pT->m_pEncodeCtx->m_pArrCodecCtx[_nSteamIndex];
    StreamContext* pEncodeStream = _pT->m_pEncodeCtx;
    return internal_write_frame(pEncodeStream, pEncoder,
                         NULL, _nSteamIndex);
}

int write_frame(TranscodeContext* _pT)
{
    int ret;
    int nStreamIndex;

    av_log(NULL, AV_LOG_INFO, "Encoding frame\n");

    ret = receive_frame(_pT->m_pDecodeCtx, &nStreamIndex);

    AVCodecContext* pEncoder= _pT->m_pEncodeCtx->m_pArrCodecCtx[nStreamIndex];
    AVCodecContext* pDecoder= _pT->m_pDecodeCtx->m_pArrCodecCtx[nStreamIndex];

    StreamContext* pDecodeStream = _pT->m_pDecodeCtx;
    StreamContext* pEncodeStream = _pT->m_pEncodeCtx;

    while (ret >= 0) {
        ret = avcodec_receive_frame(pDecoder, pDecodeStream->m_pFrame);
        if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
            av_log(NULL, AV_LOG_INFO, "%s.\n", av_err2str(ret));
            break;
        }
        else if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error: %s", av_err2str(ret));
            return ret;
        }

        pDecodeStream->m_pFrame->pts = pDecodeStream->m_pFrame->best_effort_timestamp;
        internal_write_frame(pEncodeStream, pEncoder,
                             pDecodeStream->m_pFrame, nStreamIndex);
    }

    return ret;
}

int write_trailer(TranscodeContext* _pT)
{
    return av_write_trailer(_pT->m_pEncodeCtx->m_pFmtCtx);
}

