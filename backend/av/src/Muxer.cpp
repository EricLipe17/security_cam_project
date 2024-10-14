#include "Muxer.hpp"

void Muxer::openOutput() {
    AVStream* pOutStream;
    AVStream* pInStream;
    AVCodecContext *pDecCtx, *pEncCtx;
    const AVCodec* pEncoder;
    unsigned int nIndex;

    // Added "segment" here to indicate to the FmtCtx that we are segmenting the output. Remove this
    // if using HLS.

    // This correcly segments the output, but it does not include audio.
    avformat_alloc_output_context2(&m_pFmtCtx, nullptr, "ssegment", m_szFn.c_str());
    if (!m_pFmtCtx) {
        av_log(nullptr, AV_LOG_ERROR, "Could not create output context\n");
        // return AVERROR_UNKNOWN;
    }

    for (nIndex = 0; nIndex < m_pDemuxerFmtCtx->nb_streams; nIndex++) {
        pOutStream = avformat_new_stream(m_pFmtCtx, nullptr);
        if (!pOutStream) {
            av_log(nullptr, AV_LOG_ERROR, "Failed allocating output stream\n");
            // return AVERROR_UNKNOWN;
        }

        pDecCtx = m_vDemuxerCodecCtxs.at(nIndex);

        if (pDecCtx->codec_type == AVMEDIA_TYPE_VIDEO ||
            pDecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* in this example, we choose transcoding to same codec */
            pEncoder = nullptr;
            // Using different codecs makes this really slow, but it does works.
            if (pDecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
                pEncoder = avcodec_find_encoder(pDecCtx->codec_id);
                // encoder = avcodec_find_encoder(AV_CODEC_ID_HEVC);
            } else {
                // encoder = avcodec_find_encoder(AV_CODEC_ID_AAC);
                pEncoder = avcodec_find_encoder(pDecCtx->codec_id);
            }

            if (!pEncoder) {
                av_log(nullptr, AV_LOG_FATAL, "Necessary encoder not found\n");
                // return AVERROR_INVALIDDATA;
            }
            pEncCtx = avcodec_alloc_context3(pEncoder);
            if (!pEncCtx) {
                av_log(nullptr, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
                // return AVERROR(ENOMEM);
            }

            pEncCtx->thread_count = 2;

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

            if (m_pFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
                pEncCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

            /* Third parameter can be used to pass settings to encoder */
            m_nErrCode = avcodec_open2(pEncCtx, pEncoder, nullptr);
            if (m_nErrCode < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Cannot open video encoder for stream #%u\n", nIndex);
                // return ret;
            }
            m_nErrCode = avcodec_parameters_from_context(pOutStream->codecpar, pEncCtx);
            if (m_nErrCode < 0) {
                av_log(nullptr, AV_LOG_ERROR,
                       "Failed to copy encoder parameters to output stream #%u\n", nIndex);
                // return ret;
            }

            pOutStream->time_base = pEncCtx->time_base;
            m_vCodecCtxs.push_back(pEncCtx);
        } else if (pDecCtx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(nullptr, AV_LOG_FATAL,
                   "Elementary stream #%d is of unknown type, cannot proceed\n", nIndex);
            // return AVERROR_INVALIDDATA;
        } else {
            /* if this stream must be remuxed */
            pInStream = m_pDemuxerFmtCtx->streams[nIndex];
            m_nErrCode = avcodec_parameters_copy(pOutStream->codecpar, pInStream->codecpar);
            if (m_nErrCode < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", nIndex);
                // return ret;
            }
            pOutStream->time_base = pInStream->time_base;
        }
    }
    av_dump_format(m_pFmtCtx, 0, m_szFn.c_str(), 1);

    if (!(m_pFmtCtx->oformat->flags & AVFMT_NOFILE)) {
        m_nErrCode = avio_open(&m_pFmtCtx->pb, m_szFn.c_str(), AVIO_FLAG_WRITE);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open output file '%s'", m_szFn.c_str());
            // return ret;
        }
    }

    /* init muxer, write output file header */
    AVDictionary* pOpts = nullptr;

    // Minimal HLS params to segment a file via HLS
    // av_dict_set(&pOpts, "hls_time", "10", 0);
    // av_dict_set(&pOpts, "hls_segment_size", "500000", 0);
    // av_dict_set(&pOpts, "hls_list_size", "0", 0);

    // Minimal standard container segment params
    av_dict_set(&pOpts, "segment_time", "10", 0);
    av_dict_set(&pOpts, "strftime", "1", 0);
    m_nErrCode = avformat_write_header(m_pFmtCtx, &pOpts);
    if (m_nErrCode < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
        // return ret;
    }

    // return 0;
}

int Muxer::initFilter(FilteringContext* _pFilterCtx, AVCodecContext* _pDecCtx,
                      AVCodecContext* _pEncCtx, const std::string& _szFilterSpec) {
    char args[512];
    const AVFilter* pBufferSrc = nullptr;
    const AVFilter* pBufferSink = nullptr;
    AVFilterContext* m_pBufferSrcCtx = nullptr;
    AVFilterContext* m_pBufferSinkCtx = nullptr;
    AVFilterInOut* pOutputs = avfilter_inout_alloc();
    AVFilterInOut* pInputs = avfilter_inout_alloc();
    AVFilterGraph* m_pFilterGraph = avfilter_graph_alloc();

    if (!pOutputs || !pInputs || !m_pFilterGraph) {
        m_nErrCode = AVERROR(ENOMEM);
        return m_nErrCode;
    }

    if (_pDecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
        pBufferSrc = avfilter_get_by_name("buffer");
        pBufferSink = avfilter_get_by_name("buffersink");
        if (!pBufferSrc || !pBufferSink) {
            av_log(nullptr, AV_LOG_ERROR, "filtering source or sink element not found\n");
            m_nErrCode = AVERROR_UNKNOWN;
            return m_nErrCode;
        }

        snprintf(args, sizeof(args),
                 "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d", _pDecCtx->width,
                 _pDecCtx->height, _pDecCtx->pix_fmt, _pDecCtx->time_base.num,
                 _pDecCtx->time_base.den, _pDecCtx->sample_aspect_ratio.num,
                 _pDecCtx->sample_aspect_ratio.den);

        m_nErrCode = avfilter_graph_create_filter(&m_pBufferSrcCtx, pBufferSrc, "in", args, nullptr,
                                                  m_pFilterGraph);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot create buffer source\n");
            return m_nErrCode;
        }

        m_nErrCode = avfilter_graph_create_filter(&m_pBufferSinkCtx, pBufferSink, "out", nullptr,
                                                  nullptr, m_pFilterGraph);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot create buffer sink\n");
            return m_nErrCode;
        }

        m_nErrCode = av_opt_set_bin(m_pBufferSinkCtx, "pix_fmts", (uint8_t*)&_pEncCtx->pix_fmt,
                                    sizeof(_pEncCtx->pix_fmt), AV_OPT_SEARCH_CHILDREN);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot set output pixel format\n");
            return m_nErrCode;
        }
    } else if (_pDecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
        char buf[64];
        pBufferSrc = avfilter_get_by_name("abuffer");
        pBufferSink = avfilter_get_by_name("abuffersink");
        if (!pBufferSrc || !pBufferSink) {
            av_log(nullptr, AV_LOG_ERROR, "filtering source or sink element not found\n");
            m_nErrCode = AVERROR_UNKNOWN;
            return m_nErrCode;
        }

        if (_pDecCtx->ch_layout.order == AV_CHANNEL_ORDER_UNSPEC)
            av_channel_layout_default(&_pDecCtx->ch_layout, _pDecCtx->ch_layout.nb_channels);
        av_channel_layout_describe(&_pDecCtx->ch_layout, buf, sizeof(buf));
        snprintf(args, sizeof(args),
                 "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=%s",
                 _pDecCtx->time_base.num, _pDecCtx->time_base.den, _pDecCtx->sample_rate,
                 av_get_sample_fmt_name(_pDecCtx->sample_fmt), buf);
        m_nErrCode = avfilter_graph_create_filter(&m_pBufferSrcCtx, pBufferSrc, "in", args, nullptr,
                                                  m_pFilterGraph);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot create audio buffer source\n");
            return m_nErrCode;
        }

        m_nErrCode = avfilter_graph_create_filter(&m_pBufferSinkCtx, pBufferSink, "out", nullptr,
                                                  nullptr, m_pFilterGraph);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot create audio buffer sink\n");
            return m_nErrCode;
        }

        m_nErrCode =
            av_opt_set_bin(m_pBufferSinkCtx, "sample_fmts", (uint8_t*)&_pEncCtx->sample_fmt,
                           sizeof(_pEncCtx->sample_fmt), AV_OPT_SEARCH_CHILDREN);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot set output sample format\n");
            return m_nErrCode;
        }

        av_channel_layout_describe(&_pEncCtx->ch_layout, buf, sizeof(buf));
        m_nErrCode = av_opt_set(m_pBufferSinkCtx, "ch_layouts", buf, AV_OPT_SEARCH_CHILDREN);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot set output channel layout\n");
            return m_nErrCode;
        }

        m_nErrCode =
            av_opt_set_bin(m_pBufferSinkCtx, "sample_rates", (uint8_t*)&_pEncCtx->sample_rate,
                           sizeof(_pEncCtx->sample_rate), AV_OPT_SEARCH_CHILDREN);
        if (m_nErrCode < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Cannot set output sample rate\n");
            return m_nErrCode;
        }
    } else {
        m_nErrCode = AVERROR_UNKNOWN;
        return m_nErrCode;
    }

    /* Endpoints for the filter graph. */
    pOutputs->name = av_strdup("in");
    pOutputs->filter_ctx = m_pBufferSrcCtx;
    pOutputs->pad_idx = 0;
    pOutputs->next = nullptr;

    pInputs->name = av_strdup("out");
    pInputs->filter_ctx = m_pBufferSinkCtx;
    pInputs->pad_idx = 0;
    pInputs->next = nullptr;

    if (!pOutputs->name || !pInputs->name) {
        m_nErrCode = AVERROR(ENOMEM);
        return m_nErrCode;
    }

    if ((m_nErrCode = avfilter_graph_parse_ptr(m_pFilterGraph, _szFilterSpec.c_str(), &pInputs,
                                               &pOutputs, nullptr)) < 0)
        return m_nErrCode;

    if ((m_nErrCode = avfilter_graph_config(m_pFilterGraph, nullptr)) < 0) return m_nErrCode;

    /* Fill FilteringContext */
    _pFilterCtx->m_pBufferSrcCtx = m_pBufferSrcCtx;
    _pFilterCtx->m_pBufferSinkCtx = m_pBufferSinkCtx;
    _pFilterCtx->m_pFilterGraph = m_pFilterGraph;

    avfilter_inout_free(&pInputs);
    avfilter_inout_free(&pOutputs);

    return m_nErrCode;
}

int Muxer::filterEncodeWriteFrame(AVFrame* _pFrame, const unsigned int _nStreamIndex) {
    FilteringContext filterCtx = m_vFilterCtxs.at(_nStreamIndex);

    av_log(nullptr, AV_LOG_INFO, "Pushing decoded frame to filters\n");
    /* push the decoded frame into the filtergraph */
    m_nErrCode = av_buffersrc_add_frame_flags(filterCtx.m_pBufferSrcCtx, _pFrame, 0);
    if (m_nErrCode < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
        return m_nErrCode;
    }

    /* pull filtered frames from the filtergraph */
    while (1) {
        av_log(nullptr, AV_LOG_INFO, "Pulling filtered frame from filters\n");
        m_nErrCode = av_buffersink_get_frame(filterCtx.m_pBufferSinkCtx, filterCtx.m_pFrame);
        if (m_nErrCode < 0) {
            /* if no more frames for output - returns AVERROR(EAGAIN)
             * if flushed and no more frames for output - returns AVERROR_EOF
             * rewrite retcode to 0 to show it as normal procedure completion
             */
            if (m_nErrCode == AVERROR(EAGAIN) || m_nErrCode == AVERROR_EOF) m_nErrCode = 0;
            break;
        }

        filterCtx.m_pFrame->pict_type = AV_PICTURE_TYPE_NONE;
        m_nErrCode = encodeWriteFrame(filterCtx, _nStreamIndex, 0);
        av_frame_unref(filterCtx.m_pFrame);
        if (m_nErrCode < 0) break;
    }

    return m_nErrCode;
}

int Muxer::encodeWriteFrame(FilteringContext _filterCtx, const unsigned int _nStreamIndex,
                            const int _nFlush) {
    AVFrame* pFilteredFrame = _nFlush ? nullptr : _filterCtx.m_pFrame;
    AVPacket* pPacket = _filterCtx.m_pPacket;
    AVCodecContext* pEncCtx = m_vCodecCtxs.at(_nStreamIndex);

    av_log(nullptr, AV_LOG_INFO, "Encoding frame\n");
    /* encode filtered frame */
    av_packet_unref(pPacket);

    m_nErrCode = avcodec_send_frame(pEncCtx, pFilteredFrame);

    if (m_nErrCode < 0) return m_nErrCode;

    while (m_nErrCode >= 0) {
        m_nErrCode = avcodec_receive_packet(pEncCtx, pPacket);

        if (m_nErrCode == AVERROR(EAGAIN) || m_nErrCode == AVERROR_EOF) return 0;

        /* prepare packet for muxing */
        pPacket->stream_index = _nStreamIndex;
        av_packet_rescale_ts(pPacket, pEncCtx->time_base,
                             m_pFmtCtx->streams[_nStreamIndex]->time_base);

        av_log(nullptr, AV_LOG_DEBUG, "Muxing frame\n");
        /* mux encoded frame */
        m_nErrCode = av_interleaved_write_frame(m_pFmtCtx, pPacket);
    }

    return m_nErrCode;
}

Muxer::Muxer(AVFormatContext* _pDemuxerFmtCtx, std::vector<AVCodecContext*>& _vDemuxerCodecCtxs,
             const std::string& _szFn, AVDictionary* _pOpts, const std::string& _szVideoFilterSpec,
             const std::string& _szAudioFilterSpec)
    : m_pDemuxerFmtCtx{_pDemuxerFmtCtx},
      m_vDemuxerCodecCtxs{_vDemuxerCodecCtxs},
      m_szFn{_szFn},
      m_szVideoFilterSpec{_szVideoFilterSpec},
      m_szAudioFilterSpec{_szAudioFilterSpec},
      m_nErrCode{0},
      m_pFmtCtx{nullptr} {
    openOutput();
    initFilters();
}

Muxer::Muxer(Muxer&& _muxer)
    : m_pFmtCtx{_muxer.m_pFmtCtx},
      m_vFilterCtxs{std::move(_muxer.m_vFilterCtxs)},
      m_vCodecCtxs{std::move(_muxer.m_vCodecCtxs)},
      m_pDemuxerFmtCtx{_muxer.m_pDemuxerFmtCtx},
      m_vDemuxerCodecCtxs{_muxer.m_vDemuxerCodecCtxs},
      m_szFn{std::move(_muxer.m_szFn)},
      m_szVideoFilterSpec{std::move(_muxer.m_szVideoFilterSpec)},
      m_szAudioFilterSpec{std::move(_muxer.m_szAudioFilterSpec)},
      m_nErrCode{_muxer.m_nErrCode},
      m_szErrMsg{std::move(_muxer.m_szErrMsg)} {
    _muxer.m_pFmtCtx = nullptr;
}

Muxer::~Muxer() {
    for (int i = 0; i < m_vCodecCtxs.size(); i++) {
        avcodec_free_context(&m_vCodecCtxs.at(i));
        FilteringContext filterCtx = m_vFilterCtxs.at(i);
        if (filterCtx.m_pFilterGraph) {
            avfilter_graph_free(&filterCtx.m_pFilterGraph);
            av_packet_free(&filterCtx.m_pPacket);
            av_frame_free(&filterCtx.m_pFrame);
        }
    }
    if (!(m_pFmtCtx->oformat->flags & AVFMT_NOFILE)) avio_closep(&m_pFmtCtx->pb);
    avformat_free_context(m_pFmtCtx);
}

int Muxer::initFilters() {
    std::string szFilterSpec;

    for (unsigned int i = 0; i < m_pDemuxerFmtCtx->nb_streams; i++) {
        FilteringContext filterCtx;
        filterCtx.m_pBufferSrcCtx = nullptr;
        filterCtx.m_pBufferSinkCtx = nullptr;
        filterCtx.m_pFilterGraph = nullptr;
        if (!(m_pDemuxerFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO ||
              m_pDemuxerFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO))
            continue;

        if (m_pDemuxerFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            szFilterSpec = m_szVideoFilterSpec;
        else
            szFilterSpec = m_szAudioFilterSpec;
        m_nErrCode =
            initFilter(&filterCtx, m_vDemuxerCodecCtxs.at(i), m_vCodecCtxs.at(i), szFilterSpec);
        if (m_nErrCode) return m_nErrCode;

        filterCtx.m_pPacket = av_packet_alloc();
        if (!filterCtx.m_pPacket) return AVERROR(ENOMEM);

        filterCtx.m_pFrame = av_frame_alloc();
        if (!filterCtx.m_pFrame) {
            m_nErrCode = AVERROR(ENOMEM);
            return m_nErrCode;
        }

        m_vFilterCtxs.push_back(filterCtx);
    }
    return m_nErrCode;
}

int Muxer::WriteFrame(AVFrame* _pFrame, const unsigned int _nStreamIndex) {
    return filterEncodeWriteFrame(_pFrame, _nStreamIndex);
}

int Muxer::Flush() {
    for (int i = 0; i < m_vFilterCtxs.size(); i++) {
        av_log(nullptr, AV_LOG_INFO, "Flushing stream #%u encoder\n", i);
        FilteringContext filterCtx = m_vFilterCtxs.at(i);
        m_nErrCode = encodeWriteFrame(filterCtx, i, 1);
        if (m_nErrCode < 0) return m_nErrCode;
    }
    return m_nErrCode;
}

int Muxer::CloseStream() {
    m_nErrCode = av_write_trailer(m_pFmtCtx);
    return m_nErrCode;
}
