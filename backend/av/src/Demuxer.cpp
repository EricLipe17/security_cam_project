#include "Demuxer.hpp"

#include <boost/bind/bind.hpp>

Demuxer::Demuxer(const char* _pFn, AVDictionary* _pOpts)
    : m_pFmtCtx{nullptr},
      m_pFn{_pFn},
      m_pOpts{_pOpts},
      m_pFrame{av_frame_alloc()},
      m_pPacket{av_packet_alloc()},
      m_nErrCode{0},
      m_frames{boost::bind(&Demuxer::frame, this, std::placeholders::_1)} {
    openInput();
}

Demuxer::~Demuxer() {
    for (int i = 0; i < m_vCodecCtxs.size(); i++) {
        avcodec_free_context(&m_vCodecCtxs.at(i));
        av_frame_free(&m_pFrame);
    }

    av_packet_free(&m_pPacket);

    avformat_close_input(&m_pFmtCtx);
}

void Demuxer::frame(FrameGen::push_type& yield) {
    yield(std::make_pair(Frame::INIT, nullptr));

    while (m_nErrCode >= 0) {
        av_packet_unref(m_pPacket);
        m_nErrCode = av_read_frame(m_pFmtCtx, m_pPacket);
        if (m_nErrCode < 0) yield(std::make_pair(Frame::END, nullptr));

        unsigned int nStreamIndex = m_pPacket->stream_index;
        av_log(nullptr, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n", nStreamIndex);

        AVCodecContext* pDecCtx = m_vCodecCtxs.at(nStreamIndex);

        av_packet_rescale_ts(m_pPacket, m_pFmtCtx->streams[nStreamIndex]->time_base,
                             pDecCtx->time_base);
        m_nErrCode = avcodec_send_packet(pDecCtx, m_pPacket);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Decoding failed\n");
            yield(std::make_pair(Frame::ERROR, nullptr));
        }

        while (m_nErrCode >= 0) {
            m_nErrCode = avcodec_receive_frame(pDecCtx, m_pFrame);
            if (m_nErrCode == AVERROR_EOF) yield(std::make_pair(Frame::END, nullptr));
            if (m_nErrCode == AVERROR(EAGAIN)) {
                m_nErrCode = 0;
                break;
            } else if (m_nErrCode < 0)
                yield(std::make_pair(Frame::ERROR, nullptr));

            m_pFrame->pts = m_pFrame->best_effort_timestamp;
            yield(std::make_pair(nStreamIndex, m_pFrame));
        }
    }
}

int Demuxer::openInput() {
    unsigned int nStreamIndex;

    m_nErrCode = avformat_open_input(&m_pFmtCtx, m_pFn, nullptr, nullptr);
    if (m_nErrCode < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
        return m_nErrCode;
    }

    m_nErrCode = avformat_find_stream_info(m_pFmtCtx, nullptr);
    if (m_nErrCode < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
        return m_nErrCode;
    }

    for (nStreamIndex = 0; nStreamIndex < m_pFmtCtx->nb_streams; nStreamIndex++) {
        AVStream* pStream = m_pFmtCtx->streams[nStreamIndex];
        const AVCodec* pDecoder = avcodec_find_decoder(pStream->codecpar->codec_id);
        AVCodecContext* pCodecCtx;
        if (!pDecoder) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", nStreamIndex);
            m_nErrCode = AVERROR_DECODER_NOT_FOUND;
            return m_nErrCode;
        }

        pCodecCtx = avcodec_alloc_context3(pDecoder);
        if (!pCodecCtx) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n",
                   nStreamIndex);
            m_nErrCode = AVERROR(ENOMEM);
            return m_nErrCode;
        }
        m_nErrCode = avcodec_parameters_to_context(pCodecCtx, pStream->codecpar);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR,
                   "Failed to copy decoder parameters to input decoder context "
                   "for stream #%u\n",
                   nStreamIndex);
            return m_nErrCode;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO ||
            pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO)
                pCodecCtx->framerate = av_guess_frame_rate(m_pFmtCtx, pStream, nullptr);
            /* Open decoder */
            m_nErrCode = avcodec_open2(pCodecCtx, pDecoder, nullptr);
            if (m_nErrCode < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n",
                       nStreamIndex);
                return m_nErrCode;
            }
        }
        m_vCodecCtxs.push_back(pCodecCtx);
    }

    av_dump_format(m_pFmtCtx, 0, m_pFn, 0);
    return m_nErrCode;
}
