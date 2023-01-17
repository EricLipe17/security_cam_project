#pragma once

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct StreamParams {
    char* m_pOutExt;
    char* m_pFN;
    char* m_pFQN;
} StreamParams;

typedef struct StreamContext {
    StreamParams* m_pParams;
    AVFormatContext* m_pFmtCtx;
    AVCodecContext** m_pArrCodecCtx;
    AVFrame* m_pFrame;
} StreamContext;

typedef struct TranscodeContext
{
    StreamContext* m_pInStreamCtx;
    StreamContext* m_pOutStreamCtx;
} TranscodeContext;

TranscodeContext* alloc_transcoder();
void free_transcoder(TranscodeContext* _pT);
void init_transcoder(TranscodeContext* _pT, StreamParams* _pIn, StreamParams* _pOut);
int flush_encoder(TranscodeContext* _pT, int _nSteamIndex);
int write_frame(TranscodeContext* _pT, int _nSteamIndex, int _nFlush);