// #include "RtspStream.h"
// #include "AV.h"
// #include "AVUtil.h"

// static int decompress_packet(RtspStream** _pS, const int _nCtxIndex, AVFrame** _pFrame, AVPacket** _pPkt)
// {
//     int nResponse = avcodec_send_packet((*_pS)->m_pCodecContexts[_nCtxIndex], *_pPkt);
//     if (nResponse < 0)
//     {
//         set_error_msg((*_pS)->m_pErrMsg, nResponse);
//         return PACKET_DECODE_FAILED;
//     }
//     nResponse = avcodec_receive_frame((*_pS)->m_pCodecContexts[_nCtxIndex], *_pFrame);
//     if (nResponse < 0)
//     {
//         set_error_msg((*_pS)->m_pErrMsg, nResponse);
//         return FRAME_DECODE_FAILED;
//     }

//     return NO_ERROR;
// }

// RtspStream* alloc_stream()
// {
//     RtspStream* pS = malloc(sizeof(RtspStream));
//     pS->m_pURL = "";
//     pS->m_pFormatContext = NULL;
//     pS->m_pOpts = NULL;
//     pS->m_pCodecParams = NULL;
//     pS->m_pCodecs = NULL;
//     pS->m_pCodecContexts = NULL;
//     pS->m_nNumCodecContainers = -1;
//     pS->m_pSwsScalerContext = NULL;
//     pS->m_pVideoFrame = NULL;
//     pS->m_pAudioFrame = NULL;
//     pS->m_pFrameBuffer = NULL;
//     pS->m_pAudioBuffer = NULL;
//     pS->m_nVideoStreamIndex = -1;
//     pS->m_nAudioStreamIndex = -1;
//     pS->m_pErrMsg = malloc(sizeof(char) * AV_ERROR_MAX_STRING_SIZE);
//     pS->m_pStreamWriter = alloc_stream_writer();
//     return pS;
// }

// void free_stream(RtspStream* _pS)
// {
//     _pS->m_pStreamWriter->m_nClosing = 1;
//     flush_buffer(_pS->m_pStreamWriter);

//     av_frame_free(&_pS->m_pVideoFrame);
//     _pS->m_pVideoFrame = NULL;
//     av_frame_free(&_pS->m_pAudioFrame);
//     _pS->m_pAudioFrame = NULL;
//     free(_pS->m_pFrameBuffer);
//     _pS->m_pFrameBuffer = NULL;
//     free(_pS->m_pAudioBuffer);
//     _pS->m_pAudioBuffer = NULL;

//     avformat_close_input(&_pS->m_pFormatContext);
//     avformat_free_context(_pS->m_pFormatContext);
//     _pS->m_pFormatContext = NULL;

//     for (int i = 0; i < _pS->m_nNumCodecContainers; ++i)
//     {
//         avcodec_free_context(&_pS->m_pCodecContexts[i]);
//         _pS->m_pCodecContexts[i] = NULL;
//         avcodec_parameters_free(&_pS->m_pCodecParams[i]);
//         _pS->m_pCodecParams[i] = NULL;
//     }
//     free(_pS->m_pCodecContexts);
//     _pS->m_pCodecContexts = NULL;
//     free(_pS->m_pCodecs);
//     _pS->m_pCodecs = NULL;

//     if (_pS->m_pSwsScalerContext)
//     {
//         sws_freeContext(_pS->m_pSwsScalerContext);
//         _pS->m_pSwsScalerContext = NULL;
//     }

//     av_dict_free(&_pS->m_pOpts);
//     _pS->m_pOpts = NULL;

//     free(_pS->m_pErrMsg);
//     _pS->m_pErrMsg = NULL;

//     free_stream_writer(_pS->m_pStreamWriter);
//     _pS->m_pStreamWriter = NULL;

//     free(_pS);
//     _pS = NULL;

//     // TODO: Move global ffmpeg initialization/de-initialization out of this class to avoid issues
//     avformat_network_deinit();
// }

// int init_stream(RtspStream* _pS, const char*  _pURL)
// {
//     av_log_set_level( AV_LOG_VERBOSE );
//     avformat_network_init();

//     _pS->m_pURL = _pURL;

//     // Open stream
//     _pS->m_pFormatContext = avformat_alloc_context();
//     av_dict_set(&_pS->m_pOpts, "rtsp_transport", "tcp", 0);
//     if (avformat_open_input(&_pS->m_pFormatContext, _pS->m_pURL, NULL, &_pS->m_pOpts))
//         return STREAM_OPEN_FAILED;

//     // Allocate Codec containers
//     _pS->m_nNumCodecContainers = _pS->m_pFormatContext->nb_streams;
//     _pS->m_pCodecParams = malloc(sizeof(AVCodecParameters) * _pS->m_nNumCodecContainers);
//     _pS->m_pCodecs = malloc(sizeof(AVCodec) * _pS->m_nNumCodecContainers);
//     _pS->m_pCodecContexts = malloc(sizeof(AVCodecContext) * _pS->m_nNumCodecContainers);

//     // Find the first valid video & audio stream
//     for (int i = 0; i < _pS->m_nNumCodecContainers && (_pS->m_nVideoStreamIndex + _pS->m_nAudioStreamIndex < 1); ++i)
//     {
//         AVCodecParameters* pTempParams = _pS->m_pFormatContext->streams[i]->codecpar;
//         const AVCodec* pTempCodec = avcodec_find_decoder(pTempParams->codec_id);
//         if (_pS->m_nVideoStreamIndex == -1 && pTempCodec && pTempParams->codec_type == AVMEDIA_TYPE_VIDEO)
//             _pS->m_nVideoStreamIndex = i;

//         if (_pS->m_nAudioStreamIndex == -1 && pTempCodec && pTempParams->codec_type == AVMEDIA_TYPE_AUDIO)
//             _pS->m_nAudioStreamIndex = i;

//         _pS->m_pCodecs[i] = pTempCodec;
//         _pS->m_pCodecParams[i] = pTempParams;
//     }

//     if (_pS->m_nVideoStreamIndex == -1)
//         return VIDEO_STREAM_NOT_FOUND;

//     // Setup codec context for decoder
//     _pS->m_pCodecContexts[_pS->m_nVideoStreamIndex] = avcodec_alloc_context3(_pS->m_pCodecs[_pS->m_nVideoStreamIndex]);
//     if (avcodec_parameters_to_context(_pS->m_pCodecContexts[_pS->m_nVideoStreamIndex], _pS->m_pCodecParams[_pS->m_nVideoStreamIndex]))
//         return CODEC_INIT_FAILED;

//     if (_pS->m_nAudioStreamIndex != -1)
//     {
//         _pS->m_pCodecContexts[_pS->m_nAudioStreamIndex] = avcodec_alloc_context3(_pS->m_pCodecs[_pS->m_nAudioStreamIndex]);
//         if (avcodec_parameters_to_context(_pS->m_pCodecContexts[_pS->m_nAudioStreamIndex], _pS->m_pCodecParams[_pS->m_nAudioStreamIndex]))
//             return CODEC_INIT_FAILED;
//     }

//     if (avcodec_open2(_pS->m_pCodecContexts[_pS->m_nVideoStreamIndex],
//                       _pS->m_pCodecs[_pS->m_nVideoStreamIndex], NULL) < 0)
//         return CODEC_OPEN_FAILED;

//     if (avcodec_open2(_pS->m_pCodecContexts[_pS->m_nAudioStreamIndex],
//                       _pS->m_pCodecs[_pS->m_nAudioStreamIndex], NULL) < 0)
//         return CODEC_OPEN_FAILED;

//     return init_stream_writer(_pS->m_pStreamWriter, _pS->m_pFormatContext);
// }

// int init_buffers(RtspStream* _pS, int _nMaxTries)
// {
//     AVFrame* pFrame = av_frame_alloc();
//     AVPacket* pPkt = av_packet_alloc();
//     unsigned char nRet = NO_ERROR;
//     unsigned char nFrameBufferInitialized = 0;
//     unsigned char nAudioBufferInitialized = 0;
//     while (av_read_frame(_pS->m_pFormatContext, pPkt) >= 0 && _nMaxTries && (nFrameBufferInitialized + nAudioBufferInitialized != 2))
//     {
//         --_nMaxTries;
//         if (nFrameBufferInitialized == 0 && pPkt->stream_index == _pS->m_nVideoStreamIndex)
//         {
//             nRet = decompress_packet(&_pS, _pS->m_nVideoStreamIndex, &pFrame, &pPkt);
//             if (!nRet)
//             {
//                 _pS->m_nWidth = pFrame->width;
//                 _pS->m_nHeight = pFrame->height;
//                 _pS->m_nFrameBufferSize = _pS->m_nWidth * _pS->m_nHeight * 4;
//                 _pS->m_pFrameBuffer  = malloc(sizeof(unsigned char) * _pS->m_nFrameBufferSize);
//                 ++nFrameBufferInitialized;
//             }
//         }
//         if (nAudioBufferInitialized == 0 && pPkt->stream_index == _pS->m_nAudioStreamIndex)
//         {
//             nRet = decompress_packet(&_pS, _pS->m_nAudioStreamIndex, &pFrame, &pPkt);
//             if (!nRet)
//             {
//                 _pS->m_pAudioBuffer = malloc(sizeof(unsigned char) * pFrame->linesize[0]);
//                 ++nAudioBufferInitialized;
//             }
//         }
//     }

//     clean_up_pkt_frame(&pFrame, &pPkt);

//     if (_nMaxTries == 0)
//         nRet = INIT_BUFFER_FAILED;

//     return nRet;
// }

// int get_next_frame(RtspStream* _pS)
// {
//     AVFrame* pFrame = av_frame_alloc();
//     AVPacket* pPkt = av_packet_alloc();
//     int nRet = NO_ERROR;
//     int response = av_read_frame(_pS->m_pFormatContext, pPkt);
//     if (response >= 0)
//     {
//         if (pPkt->stream_index == _pS->m_nVideoStreamIndex)
//         {
//             clean_up_frame(&_pS->m_pVideoFrame);
//             nRet = decompress_packet(&_pS, _pS->m_nVideoStreamIndex, &pFrame, &pPkt);
//             if (!nRet)
//                 _pS->m_pVideoFrame = pFrame;
//         }
//         if (pPkt->stream_index == _pS->m_nAudioStreamIndex)
//         {
//             clean_up_frame(&_pS->m_pAudioFrame);
//             nRet = decompress_packet(&_pS, _pS->m_nAudioStreamIndex, &pFrame, &pPkt);
//             if (!nRet)
//                 _pS->m_pAudioFrame = pFrame;
//         }
//         buffer_frame(_pS->m_pStreamWriter, pPkt);
//     }
//     else
//     {
//         set_error_msg(_pS->m_pErrMsg, response);
//         nRet = FRAME_DECODE_FAILED;
//     }

//     return nRet;
// }

// int format_frame(RtspStream* _pS)
// {
//     int nRet = NO_ERROR;
//     uint8_t* pDst[4] = {_pS->m_pFrameBuffer, NULL, NULL, NULL};
//                  int pDstLineSize[4] = {_pS->m_nWidth * 4, 0, 0, 0};
//     int nResponse = sws_scale(_pS->m_pSwsScalerContext, (const uint8_t *const *) _pS->m_pVideoFrame->data,
//                               _pS->m_pVideoFrame->linesize,0, _pS->m_pVideoFrame->height,
//                               pDst, pDstLineSize);
//     if (nResponse <= 0)
//     {
//         set_error_msg(_pS->m_pErrMsg, nResponse);
//         nRet = FRAME_FORMAT_FAILED;
//     }
//     return nRet;
// }

// int init_formatter(RtspStream* _pS, enum AVPixelFormat _fmt, int _nFlags, SwsFilter *_pSrcFilter,
//                    SwsFilter *_pDstFilter, const double *_pParam)
// {
//     _pS->m_pSwsScalerContext = sws_getContext(_pS->m_nWidth,
//                                               _pS->m_nHeight,
//                                               _pS->m_pCodecContexts[_pS->m_nVideoStreamIndex]->pix_fmt,
//                                               _pS->m_nWidth,
//                                               _pS->m_nHeight,
//                                               _fmt, _nFlags,
//                                               _pSrcFilter, _pDstFilter, _pParam);
// }
