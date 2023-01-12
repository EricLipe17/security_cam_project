#pragma once

#include "RtspStream.h"

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct StreamingParams
{
    char m_nRemuxVideo;
    char m_nRemuxAudio;
    char* m_pOutExt;
    char* m_pFN;
    char* m_pFQN;
    char* m_pFQP;
    char* m_pMuxerOptKey;
    char* m_pMuxerOptVal;
    enum AVCodecID m_nVideoID;
    enum AVCodecID m_nAudioID;
    char* m_pCodecPrivKey;
    char* m_pCodecPrivVal;
} StreamingParams;

typedef struct StreamContext
{
    AVFormatContext* m_pFormatCtx;
    AVCodec* m_pVideoCodec;
    AVCodec* m_pAudioCodec;
    AVStream* m_pVideoStream;
    AVStream* m_pAudioStream;
    AVCodecContext* m_pVideoCodecCtx;
    AVCodecContext* m_pAudioCodecCtx;
    int m_nVideoIndex;
    int m_nAudioIndex;
} StreamContext;

typedef struct TranscodeContext
{
    StreamContext* m_pEncoderCtx;
    StreamContext* m_pDecoderCtx;
    StreamingParams* m_pOutParams;
    AVDictionary* m_pMuxer_Opts;
    AVRational m_nSrcFrameRate;
} TranscodeContext;

TranscodeContext* alloc_transcoder();
void free_transcoder(TranscodeContext* _pT);
int init_transcoder(TranscodeContext* _pT, StreamingParams* _pParams, AVFormatContext* _pDecodeCtx, const char* _pFN);
int transcode_audio(TranscodeContext* _pT, AVPacket* _pInputPkt, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_);
int transcode_video(TranscodeContext* _pT, AVPacket* _pInputPkt, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_);
int rollover_split(TranscodeContext* _pT);