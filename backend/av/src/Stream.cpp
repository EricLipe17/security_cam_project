#include "Stream.hpp"

#include "AV.hpp"
#include "AVUtil.hpp"

int Stream::decompress_packet(const int _nCtxIndex, AVFrame** _pFrame, AVPacket** _pPkt) {
    int nResponse = avcodec_send_packet(m_vCodecContexts[_nCtxIndex], *_pPkt);
    if (nResponse < 0) {
        // set_error_msg(m_pErrMsg, nResponse);
        return PACKET_DECODE_FAILED;
    }
    nResponse = avcodec_receive_frame(m_vCodecContexts[_nCtxIndex], *_pFrame);
    if (nResponse < 0) {
        // set_error_msg(m_pErrMsg, nResponse);
        return FRAME_DECODE_FAILED;
    }

    return NO_ERROR;
}

Stream::Stream(const char* _pUrl) {
    // av_log_set_level( AV_LOG_DEBUG );

    // // TODO: Move global ffmpeg initialization/de-initialization out of this class to avoid
    // issues avformat_network_init();

    m_pUrl = _pUrl;
    m_pSwsScalerContext = nullptr;
    m_pFormatContext = nullptr;
    m_pOpts = nullptr;
    m_nNumCodecContainers = -1;
    m_pVideoFrame = nullptr;
    m_pAudioFrame = nullptr;
    m_pFrameBuffer = nullptr;
    m_pAudioBuffer = nullptr;
    m_nVideoStreamIndex = -1;
    m_nAudioStreamIndex = -1;
    m_pErrMsg = new char[AV_ERROR_MAX_STRING_SIZE];

    initStream();
    initBuffers(5);
    initFormatter(AV_PIX_FMT_RGBA, 0, nullptr, nullptr, nullptr);
}

Stream::~Stream() {
    // TODO: Verify this is actuall freeing everything
    av_frame_free(&m_pVideoFrame);
    m_pVideoFrame = nullptr;
    av_frame_free(&m_pAudioFrame);
    m_pAudioFrame = nullptr;
    free(m_pFrameBuffer);
    m_pFrameBuffer = nullptr;
    free(m_pAudioBuffer);
    m_pAudioBuffer = nullptr;

    avformat_close_input(&m_pFormatContext);
    avformat_free_context(m_pFormatContext);
    m_pFormatContext = nullptr;

    for (int i = 0; i < m_nNumCodecContainers; ++i) {
        avcodec_free_context(&m_vCodecContexts[i]);
        m_vCodecContexts[i] = nullptr;
        avcodec_parameters_free(&m_vCodecParams[i]);
        m_vCodecParams[i] = nullptr;
    }

    if (m_pSwsScalerContext) {
        sws_freeContext(m_pSwsScalerContext);
        m_pSwsScalerContext = nullptr;
    }

    av_dict_free(&m_pOpts);
    m_pOpts = nullptr;

    delete m_pErrMsg;
    m_pErrMsg = nullptr;

    // TODO: Move global ffmpeg initialization/de-initialization out of this class to avoid issues
    avformat_network_deinit();
}

int Stream::initStream() {
    // Open stream
    m_pFormatContext = avformat_alloc_context();
    av_dict_set(&m_pOpts, "rtsp_transport", "tcp", 0);
    if (avformat_open_input(&m_pFormatContext, m_pUrl, nullptr, &m_pOpts))
        return STREAM_OPEN_FAILED;

    m_nNumCodecContainers = m_pFormatContext->nb_streams;
    m_vCodecContexts.reserve(m_nNumCodecContainers);

    // Find the first valid video & audio stream
    for (int i = 0; i < m_nNumCodecContainers && (m_nVideoStreamIndex + m_nAudioStreamIndex < 1);
         ++i) {
        AVCodecParameters* pTempParams = m_pFormatContext->streams[i]->codecpar;
        const AVCodec* pTempCodec = avcodec_find_decoder(pTempParams->codec_id);
        if (m_nVideoStreamIndex == -1 && pTempCodec &&
            pTempParams->codec_type == AVMEDIA_TYPE_VIDEO)
            m_nVideoStreamIndex = i;

        if (m_nAudioStreamIndex == -1 && pTempCodec &&
            pTempParams->codec_type == AVMEDIA_TYPE_AUDIO)
            m_nAudioStreamIndex = i;

        m_vCodecs.push_back(pTempCodec);
        m_vCodecParams.push_back(pTempParams);
    }

    if (m_nVideoStreamIndex == -1) return VIDEO_STREAM_NOT_FOUND;

    // Setup codec context for decoder
    m_vCodecContexts[m_nVideoStreamIndex] = avcodec_alloc_context3(m_vCodecs[m_nVideoStreamIndex]);
    if (avcodec_parameters_to_context(m_vCodecContexts[m_nVideoStreamIndex],
                                      m_vCodecParams[m_nVideoStreamIndex]))
        return CODEC_INIT_FAILED;

    if (m_nAudioStreamIndex != -1) {
        m_vCodecContexts[m_nAudioStreamIndex] =
            avcodec_alloc_context3(m_vCodecs[m_nAudioStreamIndex]);
        if (avcodec_parameters_to_context(m_vCodecContexts[m_nAudioStreamIndex],
                                          m_vCodecParams[m_nAudioStreamIndex]))
            return CODEC_INIT_FAILED;
    }

    if (avcodec_open2(m_vCodecContexts[m_nVideoStreamIndex], m_vCodecs[m_nVideoStreamIndex],
                      nullptr) < 0)
        return CODEC_OPEN_FAILED;

    if (avcodec_open2(m_vCodecContexts[m_nAudioStreamIndex], m_vCodecs[m_nAudioStreamIndex],
                      nullptr) < 0)
        return CODEC_OPEN_FAILED;

    return 0;
}

int Stream::initBuffers(int _nMaxTries) {
    AVFrame* pFrame = av_frame_alloc();
    AVPacket* pPkt = av_packet_alloc();
    unsigned char nRet = NO_ERROR;
    unsigned char nFrameBufferInitialized = 0;
    unsigned char nAudioBufferInitialized = 0;
    while (av_read_frame(m_pFormatContext, pPkt) >= 0 && _nMaxTries &&
           (nFrameBufferInitialized + nAudioBufferInitialized != 2)) {
        --_nMaxTries;
        if (nFrameBufferInitialized == 0 && pPkt->stream_index == m_nVideoStreamIndex) {
            nRet = decompress_packet(m_nVideoStreamIndex, &pFrame, &pPkt);
            if (!nRet) {
                m_nWidth = pFrame->width;
                m_nHeight = pFrame->height;
                m_nFrameBufferSize = m_nWidth * m_nHeight * 4;
                m_pFrameBuffer = new unsigned char[m_nFrameBufferSize];
                ++nFrameBufferInitialized;
            }
        }
        if (nAudioBufferInitialized == 0 && pPkt->stream_index == m_nAudioStreamIndex) {
            nRet = decompress_packet(m_nAudioStreamIndex, &pFrame, &pPkt);
            if (!nRet) {
                m_pAudioBuffer = new unsigned char[pFrame->linesize[0]];
                ++nAudioBufferInitialized;
            }
        }
    }

    clean_up_pkt_frame(&pFrame, &pPkt);

    if (_nMaxTries == 0) nRet = INIT_BUFFER_FAILED;

    return nRet;
}

int Stream::GetNextFrame() {
    // TODO: This has a memory leak wrt the packet and frame
    AVFrame* pFrame = av_frame_alloc();
    AVPacket* pPkt = av_packet_alloc();
    int nRet = NO_ERROR;
    int response = av_read_frame(m_pFormatContext, pPkt);
    if (response >= 0) {
        if (pPkt->stream_index == m_nVideoStreamIndex) {
            clean_up_frame(&m_pVideoFrame);
            nRet = decompress_packet(m_nVideoStreamIndex, &pFrame, &pPkt);
            if (!nRet) m_pVideoFrame = pFrame;
            formatFrame();
        }
        if (pPkt->stream_index == m_nAudioStreamIndex) {
            clean_up_frame(&m_pAudioFrame);
            nRet = decompress_packet(m_nAudioStreamIndex, &pFrame, &pPkt);
            if (!nRet) m_pAudioFrame = pFrame;
        }
    } else {
        // set_error_msg(m_pErrMsg, response);
        nRet = FRAME_DECODE_FAILED;
    }

    return nRet;
}

int Stream::formatFrame() {
    int nRet = NO_ERROR;
    uint8_t* pDst[4] = {m_pFrameBuffer, nullptr, nullptr, nullptr};
    int pDstLineSize[4] = {m_nWidth * 4, 0, 0, 0};
    int nResponse =
        sws_scale(m_pSwsScalerContext, (const uint8_t* const*)m_pVideoFrame->data,
                  m_pVideoFrame->linesize, 0, m_pVideoFrame->height, pDst, pDstLineSize);
    if (nResponse <= 0) {
        // set_error_msg(m_pErrMsg, nResponse);
        nRet = FRAME_FORMAT_FAILED;
    }
    return nRet;
}

int Stream::initFormatter(enum AVPixelFormat _fmt, int _nFlags, SwsFilter* _pSrcFilter,
                          SwsFilter* _pDstFilter, const double* _pParam) {
    m_pSwsScalerContext =
        sws_getContext(m_nWidth, m_nHeight, m_vCodecContexts[m_nVideoStreamIndex]->pix_fmt,
                       m_nWidth, m_nHeight, _fmt, _nFlags, _pSrcFilter, _pDstFilter, _pParam);
    return 0;
}
