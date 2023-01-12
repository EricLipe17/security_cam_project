#pragma once

#include "Transcode.h"

#include <libavformat/avformat.h>

typedef struct StreamWriter
{
    TranscodeContext* m_pCtx;
    unsigned int m_nDesiredNumFrames;
    unsigned int m_nCurrNumFrames;
} StreamWriter;

StreamWriter* alloc_writer();
void free_writer(StreamWriter* _pSW);
int init_writer(StreamWriter* _pSW, TranscodeContext* _pCtx, int _nRolloverTime);
int write_header(StreamWriter* _pSW);
int write_trailer(StreamWriter* _pSW);
int write_packet(StreamWriter* _pSW, AVPacket** _ppPkt);

