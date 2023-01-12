//#include "AVIO.h"
//#include "AV.h"
//#include "AVUtil.h"
//
//#include <time.h>
//
//#include <libavutil/mathematics.h>
//
//#define MPEGTS_CHUNK_SIZE 10
//#define DEFAULT_FRAMERATE 50
//#define MAX_FN_LEN 41
//#define MAX_LEN_NAME 20
//
//static char* construct_filename(RtspStreamWriter* _pSW)
//{
//    time_t t = time(NULL);
//    struct tm tm = *localtime(&t);
//    snprintf(_pSW->m_pOutFile, MAX_FN_LEN,  "%s_%d-%02d-%02d_%02d:%02d:%02d.ts",
//             _pSW->m_pCamName, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
//}
//
//RtspStreamWriter* alloc_stream_writer()
//{
//    RtspStreamWriter* pSW = malloc(sizeof(RtspStreamWriter));
//    pSW->m_pOutputFmtCtx = NULL;
//    pSW->m_pInputFmtCtx = NULL;
//    pSW->m_pFrameBuffer = NULL;
//    pSW->m_pOutFile = malloc(sizeof(char) * MAX_FN_LEN);
//    pSW->m_pErrMsg = malloc(sizeof(char) * AV_ERROR_MAX_STRING_SIZE);
//    pSW->m_pCamName = malloc(sizeof(char) * MAX_LEN_NAME);
//    pSW->m_nClosing = 0;
//    return pSW;
//}
//
//void free_stream_writer(RtspStreamWriter* _pSW)
//{
//    avformat_free_context(_pSW->m_pOutputFmtCtx);
//    _pSW->m_pOutputFmtCtx = NULL;
//
//    free(_pSW->m_pFrameBuffer);
//    _pSW->m_pFrameBuffer = NULL;
//
//    free(_pSW->m_pOutFile);
//    _pSW->m_pOutFile = NULL;
//
//    free(_pSW->m_pErrMsg);
//    _pSW->m_pErrMsg = NULL;
//
//    free(_pSW->m_pCamName);
//    _pSW->m_pCamName = NULL;
//
//    free(_pSW);
//    _pSW = NULL;
//}
//
//int init_stream_writer(RtspStreamWriter* _pSW, AVFormatContext* _pInFmtCtx)
//{
//    init_buffer(_pSW);
//
//    // TODO: replace with configurable name later
//    strncpy(_pSW->m_pCamName, "Cam01", MAX_LEN_NAME);
//
//    _pSW->m_pInputFmtCtx = _pInFmtCtx;
//
//    construct_filename(_pSW);
//    int response = avformat_alloc_output_context2(&_pSW->m_pOutputFmtCtx, NULL, NULL, _pSW->m_pOutFile);
//    if (!_pSW->m_pOutputFmtCtx)
//    {
//        set_error_msg(_pSW->m_pErrMsg, response);
//        return FORMAT_CONTEXT_UNINITIALIZED;
//    }
//
//    for (int i = 0; i < _pSW->m_pInputFmtCtx->nb_streams; i++)
//    {
//        AVStream *pInStream = _pSW->m_pInputFmtCtx->streams[i];
//        AVStream *pOutStream = avformat_new_stream(_pSW->m_pOutputFmtCtx, NULL);
//
//        if (!pOutStream)
//        {
//            set_error_msg(_pSW->m_pErrMsg, response);
//            return STREAM_INIT_FAILED;
//        }
//
//        const int nMediaType = _pSW->m_pInputFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ? AVMEDIA_TYPE_VIDEO : AVMEDIA_TYPE_AUDIO;
//        AVCodecContext* pCodecCtx = avcodec_alloc_context3(nMediaType == AVMEDIA_TYPE_VIDEO ? _pSW->m_pInputFmtCtx->video_codec : _pSW->m_pInputFmtCtx->audio_codec);
//        response= avcodec_parameters_to_context(pCodecCtx, pInStream->codecpar);
//        if (response < 0 || !pCodecCtx)
//        {
//            set_error_msg(_pSW->m_pErrMsg, response);
//            return CODEC_INIT_FAILED;
//        }
//
//        pCodecCtx->codec_tag = 0;
//        if (_pSW->m_pOutputFmtCtx->oformat->flags & AVFMT_GLOBALHEADER)
//            pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//
//        response = avcodec_parameters_from_context(pOutStream->codecpar, pCodecCtx);
//        if (response < 0)
//        {
//            set_error_msg(_pSW->m_pErrMsg, response);
//            return CODEC_INIT_FAILED;
//        }
//    }
//
//    if (!(_pSW->m_pOutputFmtCtx->flags & AVFMT_NOFILE)) {
//        response = avio_open(&_pSW->m_pOutputFmtCtx->pb, _pSW->m_pOutFile, AVIO_FLAG_WRITE);
//        if (response < 0)
//        {
//            set_error_msg(_pSW->m_pErrMsg, response);
//            return OUTPUT_FILE_OPEN_FAILED;
//        }
//    }
//
//    response = avformat_write_header(_pSW->m_pOutputFmtCtx, NULL);
//    _pSW->m_nHeaderWritten = 1;
//    if (response < 0)
//    {
//        set_error_msg(_pSW->m_pErrMsg, response);
//        return HEADER_WRITE_FAILED;
//    }
//
//    return NO_ERROR;
//}
//
//int buffer_frame(RtspStreamWriter* _pSW, AVPacket* _pPkt)
//{
//    if (_pSW->m_nMaxBufferSize == _pSW->m_nCurrBufferSize)
//        flush_buffer(_pSW);
//    _pSW->m_pFrameBuffer[_pSW->m_nCurrBufferSize++] = _pPkt;
//}
//
//int init_buffer(RtspStreamWriter* _pSW)
//{
//    // TODO: determine the framerate of the stream to make this dynamic, only use default if it cant be computed.
//    _pSW->m_pFrameBuffer = malloc(sizeof(AVPacket) * DEFAULT_FRAMERATE * MPEGTS_CHUNK_SIZE);
//    _pSW->m_nMaxBufferSize = DEFAULT_FRAMERATE * MPEGTS_CHUNK_SIZE;
//    _pSW->m_nCurrBufferSize = 0;
//}
//
//int flush_buffer(RtspStreamWriter* _pSW)
//{
//    int response;
//    int nRet = NO_ERROR;
//
//    for (int i = 0; i < _pSW->m_nCurrBufferSize; ++i)
//    {
//        AVPacket* pPkt = _pSW->m_pFrameBuffer[i];
//        AVStream *pInStream, *pOutStream;
//        pInStream  = _pSW->m_pInputFmtCtx->streams[pPkt->stream_index];
//        pOutStream = _pSW->m_pOutputFmtCtx->streams[pPkt->stream_index];
//        /* copy packet */
//        pPkt->pts = av_rescale_q_rnd(pPkt->pts, pInStream->time_base, pOutStream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
//        pPkt->dts = av_rescale_q_rnd(pPkt->dts, pInStream->time_base, pOutStream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
//        pPkt->duration = av_rescale_q(pPkt->duration, pInStream->time_base, pOutStream->time_base);
//        pPkt->pos = -1;
//        response = av_interleaved_write_frame(_pSW->m_pOutputFmtCtx, pPkt);
//        if (response < 0)
//        {
//            nRet = WRITE_FRAME_FAILED;
//            set_error_msg(_pSW->m_pErrMsg, response);
//        }
//
//        clean_up_pkt(&pPkt);
//    }
//    av_write_trailer(_pSW->m_pOutputFmtCtx);
//    if (_pSW->m_pOutputFmtCtx && !(_pSW->m_pOutputFmtCtx->oformat->flags & AVFMT_NOFILE))
//        avio_close(_pSW->m_pOutputFmtCtx->pb);
//    _pSW->m_nHeaderWritten = 0;
//    _pSW->m_nCurrBufferSize = 0;
//
//    rollover_output_file(_pSW);
//    _pSW->m_nCurrBufferSize = 0;
//
//    return nRet;
//}
//
//int rollover_output_file(RtspStreamWriter* _pSW)
//{
//    if (_pSW->m_nClosing)
//        return NO_ERROR;
//
//    int response;
//    construct_filename(_pSW);
//    _pSW->m_pOutputFmtCtx->url = _pSW->m_pOutFile;
//
//    if (!(_pSW->m_pOutputFmtCtx->flags & AVFMT_NOFILE)) {
//        response = avio_open(&_pSW->m_pOutputFmtCtx->pb, _pSW->m_pOutFile, AVIO_FLAG_WRITE);
//        if (response < 0)
//        {
//            set_error_msg(_pSW->m_pErrMsg, response);
//            return OUTPUT_FILE_OPEN_FAILED;
//        }
//    }
//
//    response = avformat_write_header(_pSW->m_pOutputFmtCtx, NULL);
//    _pSW->m_nHeaderWritten = 1;
//    if (response < 0)
//    {
//        set_error_msg(_pSW->m_pErrMsg, response);
//        return HEADER_WRITE_FAILED;
//    }
//
//    return NO_ERROR;
//}
