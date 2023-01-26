#pragma once

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct StreamParams {
    char* m_pOutExt;
    char* m_pFN;
    char* m_pFQN;
} StreamParams;

typedef struct CodecOpts {
    AVDictionary* m_pOpts;
    enum AVCodecID m_nVideoCodecID;
    enum AVCodecID m_nAudioCodecID;
    int m_nFlags;
    int m_nFlags2;
} CodecOpts;

typedef struct StreamContext {
    StreamParams* m_pParams;
    AVFormatContext* m_pFmtCtx;
    AVCodecContext** m_pArrCodecCtx;
    AVFrame* m_pFrame;
    AVPacket* m_pPkt;
} StreamContext;

typedef struct TranscodeContext
{
    StreamContext* m_pDecodeCtx;
    StreamContext* m_pEncodeCtx;
    CodecOpts* m_pOpts;
} TranscodeContext;

TranscodeContext* alloc_transcoder();
void free_transcoder(TranscodeContext* _pT);
void init_transcoder(TranscodeContext* _pT, StreamParams* _pIn, StreamParams* _pOut);
int flush_encoder(TranscodeContext* _pT, int _nSteamIndex);
int write_frame(TranscodeContext* _pT);
int write_trailer(TranscodeContext* _pT);
void set_encode_options(TranscodeContext* _pT, AVDictionary* _pOpts);
int add_encode_option(TranscodeContext* _pT, const char* _pKey, const char* _pValue, int _nFlags);
void set_video_encode_id(TranscodeContext* _pT, enum AVCodecID _nID);
void set_audio_encode_id(TranscodeContext* _pT, enum AVCodecID _nID);
void set_encode_flags(TranscodeContext* _pT, int _nFlags, int _nFlags2);
void add_encode_flag(TranscodeContext* _pT, int _nFlag, int _nFlag2);
