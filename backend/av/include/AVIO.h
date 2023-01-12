#pragma once

#include <libavformat/avformat.h>

typedef struct RtspStreamWriter
{
    AVFormatContext* m_pOutputFmtCtx;
    AVFormatContext* m_pInputFmtCtx;
    AVPacket** m_pFrameBuffer;
    unsigned int m_nMaxBufferSize;
    unsigned int m_nCurrBufferSize;
    char* m_pOutFile;
    char* m_pErrMsg;
    char* m_pCamName;
    int8_t m_nHeaderWritten;
    int8_t m_nClosing;
} RtspStreamWriter;

RtspStreamWriter* alloc_stream_writer();
void free_stream_writer(RtspStreamWriter* _pSW);

int init_stream_writer(RtspStreamWriter* _pSW, AVFormatContext* _pInFmtCtx);
int buffer_frame(RtspStreamWriter* _pSW, AVPacket* _pPkt);
int init_buffer(RtspStreamWriter* _pSW);
int flush_buffer(RtspStreamWriter* _pSW);
int rollover_output_file(RtspStreamWriter* _pSW);
