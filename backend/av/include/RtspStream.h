#pragma once

#include "AVIO.h"

#include <stdbool.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>

typedef struct RtspStream
{
    const char*  m_pURL;
    AVFormatContext *m_pFormatContext;
    AVDictionary *m_pOpts;
    AVCodecParameters** m_pCodecParams;
    const AVCodec** m_pCodecs;
    AVCodecContext** m_pCodecContexts;
    struct SwsContext* m_pSwsScalerContext;
    unsigned int m_nNumCodecContainers;
    AVFrame* m_pVideoFrame;
    AVFrame* m_pAudioFrame;
    unsigned char* m_pFrameBuffer;
    unsigned char* m_pAudioBuffer;
    unsigned long m_nFrameBufferSize;
    int m_nHeight;
    int m_nWidth;
    int m_nVideoStreamIndex;
    int m_nAudioStreamIndex;
    char* m_pErrMsg;
    RtspStreamWriter* m_pStreamWriter;
} RtspStream;

RtspStream* alloc_stream();
void free_stream(RtspStream* _pS);

int init_stream(RtspStream* _pS, const char*  _pURL);
int init_buffers(RtspStream* _pS, int _nMaxTries);
int get_next_frame(RtspStream* _pS);
int format_frame(RtspStream* _pS);
int init_formatter(RtspStream* _pS, enum AVPixelFormat _fmt, int _nFlags, SwsFilter *_pSrcFilter,
                   SwsFilter *_pDstFilter, const double *_pParam);
