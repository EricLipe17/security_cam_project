#pragma once

#include "AVUtil.hpp"
#include "Muxer.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

class Demuxer {
   public:
    AVFormatContext* m_pFmtCtx;
    std::vector<AVCodecContext*> m_vCodecCtxs;
    // FrameBuffer m_buffer;
    AVFrame* m_pFrame;
    AVPacket* m_pPacket;
    AVDictionary* m_pOpts;
    const char* m_pFn;
    int m_nErrCode;
    std::string m_szErrMsg;

    int openInput();

   public:
    Demuxer(const char* _pFn, AVDictionary* _pOpts);
    ~Demuxer();
    int Frame(Muxer& _muxer);
};
