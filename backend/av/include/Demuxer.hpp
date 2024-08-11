#pragma once

#include "AVUtil.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

class Demuxer {
   private:
    AVFormatContext* m_pFmtCtx;
    std::vector<AVCodecContext*> m_vCodecCtxs;
    FrameBuffer m_buffer;
    AVPacket* m_pPacket;
    AVDictionary* m_pOpts;
    const char* m_pFn;
    int m_nErrCode;
    std::string m_szErrMsg;

    void openInput();

   public:
    Demuxer(const char* _pFn, AVDictionary* _pOpts);
    ~Demuxer();
    void Frame();
};
