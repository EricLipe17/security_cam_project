#pragma once

#include <libavcodec/avcodec.h>

static void set_error_msg(char* _pErrMsg, const int _nResponse)
{
    memcpy(_pErrMsg, av_err2str(_nResponse), AV_ERROR_MAX_STRING_SIZE);
}

static void clean_up_pkt_frame(AVFrame** _pFrame, AVPacket** _pPkt)
{
    av_packet_unref(*_pPkt);
    av_packet_free(_pPkt);
    av_frame_free(_pFrame);
}

static void clean_up_frame(AVFrame** _pFrame)
{
    av_frame_free(_pFrame);
}

static void clean_up_pkt(AVPacket** _pPkt)
{
    av_packet_unref(*_pPkt);
    av_packet_free(_pPkt);
}

static void get_next_filename(const char* _pFN, const char* _pExt, char** _pDst)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    const unsigned char nMaxSize = 50;
    if (!(*_pDst) || strlen((*_pDst)) < nMaxSize)
    {
        (*_pDst) = malloc(sizeof(char) * nMaxSize);
    }
    snprintf(*_pDst, nMaxSize, "%s-%d-%02d-%02d_%02d:%02d:%02d.%s",
             _pFN, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, _pExt);
}