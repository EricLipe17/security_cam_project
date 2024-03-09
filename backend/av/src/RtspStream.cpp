#include "RtspStream.h"
#include "AV.h"
#include "AVUtil.h"

int RtspStream::decompress_packet(const int _nCtxIndex, AVFrame** _pFrame, AVPacket** _pPkt)
{
    int nResponse = avcodec_send_packet(m_pCodecContexts[_nCtxIndex], *_pPkt);
    if (nResponse < 0)
    {
        set_error_msg(m_pErrMsg, nResponse);
        return PACKET_DECODE_FAILED;
    }
    nResponse = avcodec_receive_frame(m_pCodecContexts[_nCtxIndex], *_pFrame);
    if (nResponse < 0)
    {
        set_error_msg(m_pErrMsg, nResponse);
        return FRAME_DECODE_FAILED;
    }

    return NO_ERROR;
}

RtspStream::RtspStream(const char* _pUrl) 
{
    av_log_set_level( AV_LOG_VERBOSE );
    
    // TODO: Move global ffmpeg initialization/de-initialization out of this class to avoid issues
    avformat_network_init();

    m_pURL = _pUrl;

    // Allocate Codec containers
    m_nNumCodecContainers = m_pFormatContext->nb_streams;
    m_pCodecParams = new AVCodecParameters*[m_nNumCodecContainers];
    m_pCodecs = new AVCodec*[m_nNumCodecContainers];
    m_pCodecContexts = new AVCodecContext*[m_nNumCodecContainers];

    m_pFormatContext = NULL;
    m_pOpts = NULL;
    m_pCodecParams = NULL;
    m_pCodecs = NULL;
    m_pCodecContexts = NULL;
    m_nNumCodecContainers = -1;
    m_pSwsScalerContext = NULL;
    m_pVideoFrame = NULL;
    m_pAudioFrame = NULL;
    m_pFrameBuffer = NULL;
    m_pAudioBuffer = NULL;
    m_nVideoStreamIndex = -1;
    m_nAudioStreamIndex = -1;
    m_pErrMsg = new char[AV_ERROR_MAX_STRING_SIZE];
}

RtspStream::~RtspStream()
{
    // TODO: Verify this is actuall freeing everything
    av_frame_free(&m_pVideoFrame);
    m_pVideoFrame = NULL;
    av_frame_free(&m_pAudioFrame);
    m_pAudioFrame = NULL;
    free(m_pFrameBuffer);
    m_pFrameBuffer = NULL;
    free(m_pAudioBuffer);
    m_pAudioBuffer = NULL;

    avformat_close_input(&m_pFormatContext);
    avformat_free_context(m_pFormatContext);
    m_pFormatContext = NULL;

    for (int i = 0; i < m_nNumCodecContainers; ++i)
    {
        avcodec_free_context(&m_pCodecContexts[i]);
        m_pCodecContexts[i] = NULL;
        avcodec_parameters_free(&m_pCodecParams[i]);
        m_pCodecParams[i] = NULL;
    }
    free(m_pCodecContexts);
    m_pCodecContexts = NULL;
    free(m_pCodecs);
    m_pCodecs = NULL;

    if (m_pSwsScalerContext)
    {
        sws_freeContext(m_pSwsScalerContext);
        m_pSwsScalerContext = NULL;
    }

    av_dict_free(&m_pOpts);
    m_pOpts = NULL;

    delete m_pErrMsg;
    m_pErrMsg = NULL;

    // TODO: Move global ffmpeg initialization/de-initialization out of this class to avoid issues
    avformat_network_deinit();
}

int RtspStream::init_stream()
{
    // Open stream
    m_pFormatContext = avformat_alloc_context();
    av_dict_set(&m_pOpts, "rtsp_transport", "tcp", 0);
    if (avformat_open_input(&m_pFormatContext, m_pURL, NULL, &m_pOpts))
        return STREAM_OPEN_FAILED;
    
    // Find the first valid video & audio stream
    for (int i = 0; i < m_nNumCodecContainers && (m_nVideoStreamIndex + m_nAudioStreamIndex < 1); ++i)
    {
        AVCodecParameters* pTempParams = m_pFormatContext->streams[i]->codecpar;
        const AVCodec* pTempCodec = avcodec_find_decoder(pTempParams->codec_id);
        if (m_nVideoStreamIndex == -1 && pTempCodec && pTempParams->codec_type == AVMEDIA_TYPE_VIDEO)
            m_nVideoStreamIndex = i;

        if (m_nAudioStreamIndex == -1 && pTempCodec && pTempParams->codec_type == AVMEDIA_TYPE_AUDIO)
            m_nAudioStreamIndex = i;

        m_pCodecs[i] = pTempCodec;
        m_pCodecParams[i] = pTempParams;
    }

    if (m_nVideoStreamIndex == -1)
        return VIDEO_STREAM_NOT_FOUND;

    // Setup codec context for decoder
    m_pCodecContexts[m_nVideoStreamIndex] = avcodec_alloc_context3(m_pCodecs[m_nVideoStreamIndex]);
    if (avcodec_parameters_to_context(m_pCodecContexts[m_nVideoStreamIndex], m_pCodecParams[m_nVideoStreamIndex]))
        return CODEC_INIT_FAILED;

    if (m_nAudioStreamIndex != -1)
    {
        m_pCodecContexts[m_nAudioStreamIndex] = avcodec_alloc_context3(m_pCodecs[m_nAudioStreamIndex]);
        if (avcodec_parameters_to_context(m_pCodecContexts[m_nAudioStreamIndex], m_pCodecParams[m_nAudioStreamIndex]))
            return CODEC_INIT_FAILED;
    }

    if (avcodec_open2(m_pCodecContexts[m_nVideoStreamIndex],
                      m_pCodecs[m_nVideoStreamIndex], NULL) < 0)
        return CODEC_OPEN_FAILED;

    if (avcodec_open2(m_pCodecContexts[m_nAudioStreamIndex],
                      m_pCodecs[m_nAudioStreamIndex], NULL) < 0)
        return CODEC_OPEN_FAILED;

    return 0;
}

int init_buffers(RtspStream* _pS, int _nMaxTries)
{
    AVFrame* pFrame = av_frame_alloc();
    AVPacket* pPkt = av_packet_alloc();
    unsigned char nRet = NO_ERROR;
    unsigned char nFrameBufferInitialized = 0;
    unsigned char nAudioBufferInitialized = 0;
    while (av_read_frame(m_pFormatContext, pPkt) >= 0 && _nMaxTries && (nFrameBufferInitialized + nAudioBufferInitialized != 2))
    {
        --_nMaxTries;
        if (nFrameBufferInitialized == 0 && pPkt->stream_index == m_nVideoStreamIndex)
        {
            nRet = decompress_packet(&_pS, m_nVideoStreamIndex, &pFrame, &pPkt);
            if (!nRet)
            {
                m_nWidth = pFrame->width;
                m_nHeight = pFrame->height;
                m_nFrameBufferSize = m_nWidth * m_nHeight * 4;
                m_pFrameBuffer  = malloc(sizeof(unsigned char) * m_nFrameBufferSize);
                ++nFrameBufferInitialized;
            }
        }
        if (nAudioBufferInitialized == 0 && pPkt->stream_index == m_nAudioStreamIndex)
        {
            nRet = decompress_packet(&_pS, m_nAudioStreamIndex, &pFrame, &pPkt);
            if (!nRet)
            {
                m_pAudioBuffer = malloc(sizeof(unsigned char) * pFrame->linesize[0]);
                ++nAudioBufferInitialized;
            }
        }
    }

    clean_up_pkt_frame(&pFrame, &pPkt);

    if (_nMaxTries == 0)
        nRet = INIT_BUFFER_FAILED;

    return nRet;
}

int get_next_frame(RtspStream* _pS)
{
    AVFrame* pFrame = av_frame_alloc();
    AVPacket* pPkt = av_packet_alloc();
    int nRet = NO_ERROR;
    int response = av_read_frame(m_pFormatContext, pPkt);
    if (response >= 0)
    {
        if (pPkt->stream_index == m_nVideoStreamIndex)
        {
            clean_up_frame(&m_pVideoFrame);
            nRet = decompress_packet(&_pS, m_nVideoStreamIndex, &pFrame, &pPkt);
            if (!nRet)
                m_pVideoFrame = pFrame;
        }
        if (pPkt->stream_index == m_nAudioStreamIndex)
        {
            clean_up_frame(&m_pAudioFrame);
            nRet = decompress_packet(&_pS, m_nAudioStreamIndex, &pFrame, &pPkt);
            if (!nRet)
                m_pAudioFrame = pFrame;
        }
        buffer_frame(m_pStreamWriter, pPkt);
    }
    else
    {
        set_error_msg(m_pErrMsg, response);
        nRet = FRAME_DECODE_FAILED;
    }

    return nRet;
}

int format_frame(RtspStream* _pS)
{
    int nRet = NO_ERROR;
    uint8_t* pDst[4] = {m_pFrameBuffer, NULL, NULL, NULL};
                 int pDstLineSize[4] = {m_nWidth * 4, 0, 0, 0};
    int nResponse = sws_scale(m_pSwsScalerContext, (const uint8_t *const *) m_pVideoFrame->data,
                              m_pVideoFrame->linesize,0, m_pVideoFrame->height,
                              pDst, pDstLineSize);
    if (nResponse <= 0)
    {
        set_error_msg(m_pErrMsg, nResponse);
        nRet = FRAME_FORMAT_FAILED;
    }
    return nRet;
}

int init_formatter(RtspStream* _pS, enum AVPixelFormat _fmt, int _nFlags, SwsFilter *_pSrcFilter,
                   SwsFilter *_pDstFilter, const double *_pParam)
{
    m_pSwsScalerContext = sws_getContext(m_nWidth,
                                              m_nHeight,
                                              m_pCodecContexts[m_nVideoStreamIndex]->pix_fmt,
                                              m_nWidth,
                                              m_nHeight,
                                              _fmt, _nFlags,
                                              _pSrcFilter, _pDstFilter, _pParam);
}
