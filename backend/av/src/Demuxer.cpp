#include "Demuxer.hpp"

Demuxer::Demuxer(const char* _pFn, AVDictionary* _pOpts)
    : m_pInFmtCtx{nullptr},
      m_pFn{_pFn},
      m_pOpts{_pOpts},
      m_vDecCodecCtxs{nullptr, nullptr},
      m_vDecFrames{nullptr, nullptr},
      m_nErrCode{0} {
    openInput();
}

Demuxer::~Demuxer() {
    for (int i = 0; i < m_vDecCodecCtxs.size(); i++) {
        avcodec_free_context(&m_vDecCodecCtxs.at(i));
        av_frame_free(&m_vDecFrames.at(i));
    }

    avformat_close_input(&m_pInFmtCtx);
}

void Demuxer::Frame(AVPacket* _pPacket) {
    m_nErrCode = av_read_frame(m_pInFmtCtx, _pPacket);
    if (m_nErrCode < 0) return;

    unsigned int nStreamIndex = _pPacket->stream_index;
    av_log(NULL, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n", nStreamIndex);

    AVCodecContext* pDecCtx = m_vDecCodecCtxs.at(nStreamIndex);
    AVFrame* pFrame = m_vDecFrames.at(nStreamIndex);

    av_packet_rescale_ts(_pPacket, m_pInFmtCtx->streams[nStreamIndex]->time_base,
                         pDecCtx->time_base);
    m_nErrCode = avcodec_send_packet(pDecCtx, _pPacket);
    if (m_nErrCode < 0) {
        av_log(NULL, AV_LOG_ERROR, "Decoding failed\n");
        return;
    }

    while (m_nErrCode >= 0) {
        m_nErrCode = avcodec_receive_frame(pDecCtx, pFrame);
        if (m_nErrCode == AVERROR_EOF || m_nErrCode == AVERROR(EAGAIN))
            return;
        else if (m_nErrCode < 0)
            exit(1);

        pFrame->pts = pFrame->best_effort_timestamp;
        // TODO: the muxer needs to get the frame at this point in time to write it.
    }
}

void Demuxer::openInput() {
    unsigned int nStreamIndex;

    m_nErrCode = avformat_open_input(&m_pInFmtCtx, m_pFn, NULL, NULL);
    if (m_nErrCode < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return;
    }

    m_nErrCode = avformat_find_stream_info(m_pInFmtCtx, NULL);
    if (m_nErrCode < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return;
    }

    if (m_pInFmtCtx->nb_streams > m_vDecCodecCtxs.size()) {
        m_vDecCodecCtxs.reserve(m_pInFmtCtx->nb_streams);
        m_vDecFrames.reserve(m_pInFmtCtx->nb_streams);
    }

    for (nStreamIndex = 0; nStreamIndex < m_pInFmtCtx->nb_streams; nStreamIndex++) {
        AVStream* pStream = m_pInFmtCtx->streams[nStreamIndex];
        const AVCodec* pDecoder = avcodec_find_decoder(pStream->codecpar->codec_id);
        AVCodecContext* pCodecCtx;
        if (!pDecoder) {
            av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", nStreamIndex);
            m_nErrCode = AVERROR_DECODER_NOT_FOUND;
            return;
        }

        pCodecCtx = avcodec_alloc_context3(pDecoder);
        if (!pCodecCtx) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n",
                   nStreamIndex);
            m_nErrCode = AVERROR(ENOMEM);
            return;
        }
        m_nErrCode = avcodec_parameters_to_context(pCodecCtx, pStream->codecpar);
        if (m_nErrCode < 0) {
            av_log(NULL, AV_LOG_ERROR,
                   "Failed to copy decoder parameters to input decoder context "
                   "for stream #%u\n",
                   nStreamIndex);
            return;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO ||
            pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO)
                pCodecCtx->framerate = av_guess_frame_rate(m_pInFmtCtx, pStream, NULL);
            /* Open decoder */
            m_nErrCode = avcodec_open2(pCodecCtx, pDecoder, NULL);
            if (m_nErrCode < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", nStreamIndex);
                return;
            }
        }
        m_vDecCodecCtxs[nStreamIndex] = pCodecCtx;

        m_vDecFrames[nStreamIndex] = av_frame_alloc();
        if (m_vDecFrames[nStreamIndex]) {
            m_nErrCode = AVERROR(ENOMEM);
            return;
        }
    }

    av_dump_format(m_pInFmtCtx, 0, m_pFn, 0);
}
