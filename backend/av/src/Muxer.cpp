#include "Muxer.hpp"

void Muxer::openInput() {
    AVStream* pOutStream;
    AVStream* pInStream;
    AVCodecContext *pDecCtx, *pEncCtx;
    const AVCodec* pEncoder;
    unsigned int nIndex;

    avformat_alloc_output_context2(&m_pOutFmtCtx, NULL, NULL, m_pFn);
    if (!m_pOutFmtCtx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        // return AVERROR_UNKNOWN;
    }

    for (nIndex = 0; nIndex < m_pDemuxerFmtCtx->nb_streams; nIndex++) {
        pOutStream = avformat_new_stream(m_pOutFmtCtx, NULL);
        if (!pOutStream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            // return AVERROR_UNKNOWN;
        }

        pDecCtx = m_vDemuxerCodecCtxs.at(nIndex);

        if (pDecCtx->codec_type == AVMEDIA_TYPE_VIDEO ||
            pDecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* in this example, we choose transcoding to same codec */
            pEncoder = NULL;
            // Using different codecs makes this really slow, but it does works.
            if (pDecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
                pEncoder = avcodec_find_encoder(pDecCtx->codec_id);
                // encoder = avcodec_find_encoder(AV_CODEC_ID_HEVC);
            } else {
                // encoder = avcodec_find_encoder(AV_CODEC_ID_AAC);
                pEncoder = avcodec_find_encoder(pDecCtx->codec_id);
            }

            if (!pEncoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                // return AVERROR_INVALIDDATA;
            }
            pEncCtx = avcodec_alloc_context3(pEncoder);
            if (!pEncCtx) {
                av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
                // return AVERROR(ENOMEM);
            }

            pEncCtx->thread_count = 6;

            /* In this example, we transcode to same properties (picture size,
             * sample rate etc.). These properties can be changed for output
             * streams easily using filters */
            if (pDecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
                pEncCtx->height = pDecCtx->height;
                pEncCtx->width = pDecCtx->width;
                pEncCtx->sample_aspect_ratio = pDecCtx->sample_aspect_ratio;
                pEncCtx->bit_rate = pDecCtx->bit_rate;
                pEncCtx->framerate = pDecCtx->framerate;
                pEncCtx->gop_size = (int)(2 * pEncCtx->framerate.num / pEncCtx->framerate.den) + 1;
                pEncCtx->flags = AV_CODEC_FLAG_OUTPUT_CORRUPT & AV_CODEC_FLAG_CLOSED_GOP;
                /* take first format from list of supported formats */
                if (pEncoder->pix_fmts)
                    pEncCtx->pix_fmt = pEncoder->pix_fmts[0];
                else
                    pEncCtx->pix_fmt = pDecCtx->pix_fmt;
                /* video time_base can be set to whatever is handy and supported by
                 * encoder */
                pEncCtx->time_base = pDecCtx->time_base;
            } else {
                pEncCtx->sample_rate = pDecCtx->sample_rate;
                m_nErrCode = av_channel_layout_copy(&pEncCtx->ch_layout, &pDecCtx->ch_layout);
                // if (ret < 0) return ret;
                /* take first format from list of supported formats */
                pEncCtx->sample_fmt = pEncoder->sample_fmts[0];
                pEncCtx->time_base = (AVRational){1, pEncCtx->sample_rate};
            }

            if (m_pOutFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
                pEncCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            /* Third parameter can be used to pass settings to encoder */
            m_nErrCode = avcodec_open2(pEncCtx, pEncoder, NULL);
            if (m_nErrCode < 0) {
                av_log(NULL, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", nIndex);
                // return ret;
            }
            m_nErrCode = avcodec_parameters_from_context(pOutStream->codecpar, pEncCtx);
            if (m_nErrCode < 0) {
                av_log(NULL, AV_LOG_ERROR,
                       "Failed to copy encoder parameters to output stream #%u\n", nIndex);
                // return ret;
            }

            pOutStream->time_base = pEncCtx->time_base;
            m_vCodecCtxs.push_back(pEncCtx);
        } else if (pDecCtx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n",
                   nIndex);
            // return AVERROR_INVALIDDATA;
        } else {
            /* if this stream must be remuxed */
            pInStream = m_pDemuxerFmtCtx->streams[nIndex];
            m_nErrCode = avcodec_parameters_copy(pOutStream->codecpar, pInStream->codecpar);
            if (m_nErrCode < 0) {
                av_log(NULL, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", nIndex);
                // return ret;
            }
            pOutStream->time_base = pInStream->time_base;
        }
    }
    av_dump_format(m_pOutFmtCtx, 0, m_pFn, 1);

    if (!(m_pOutFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        m_nErrCode = avio_open(&m_pOutFmtCtx->pb, m_pFn, AVIO_FLAG_WRITE);
        if (m_nErrCode < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", m_pFn);
            // return ret;
        }
    }

    /* init muxer, write output file header */
    AVDictionary* pOpts = NULL;

    // Minimal HLS params to segment a file via HLS
    av_dict_set(&pOpts, "hls_time", "10", 0);
    av_dict_set(&pOpts, "hls_segment_size", "500000", 0);
    av_dict_set(&pOpts, "hls_list_size", "0", 0);
    m_nErrCode = avformat_write_header(m_pOutFmtCtx, &pOpts);
    if (m_nErrCode < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
        // return ret;
    }

    // return 0;
}

void Muxer::initFilter() {}

void Muxer::filterEncodeWriteFrame() {}

void Muxer::encodeWriteFrame() {}

Muxer::Muxer(AVFormatContext* _pDemuxerFmtCtx, std::vector<AVCodecContext*>& _vDemuxerCodecCtxs,
             const char* _pFn, AVDictionary* _pOpts)
    : m_pDemuxerFmtCtx{_pDemuxerFmtCtx},
      m_vDemuxerCodecCtxs{m_vDemuxerCodecCtxs},
      m_pFn{_pFn},
      m_nErrCode{0},
      m_pOutFmtCtx{NULL},
      m_pEncCodecCtx{NULL} {}

// TODO: Free everything!
Muxer::~Muxer() {}

void Muxer::InitFilters() {}

void Muxer::WriteBuffer(const FrameBuffer& _demuxerBuf) {}

void Muxer::Flush() {}
