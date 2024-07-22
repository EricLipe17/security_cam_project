#pragma once

#include <ctime>
#include <string>

extern "C" {
    #include <libavcodec/avcodec.h>
}

static std::string get_error_msg(const int _nResponse)
{
    std::string szResponse(AV_ERROR_MAX_STRING_SIZE, 'x');
    av_strerror(_nResponse, const_cast<char*>(szResponse.c_str()), AV_ERROR_MAX_STRING_SIZE);
    return szResponse;
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
    const unsigned char nMaxSize = strlen(_pFN) + strlen(_pExt) + 18;
    if (!(*_pDst) || strlen((*_pDst)) < nMaxSize)
    {
        (*_pDst) = new char[nMaxSize]();
    }
    snprintf(*_pDst, nMaxSize, "%s-%d-%02d-%02d_%02d:%02d:%02d.%s\n",
             _pFN, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, _pExt);
}
