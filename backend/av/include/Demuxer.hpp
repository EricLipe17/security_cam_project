#pragma once

#include "AVUtil.hpp"
#include "Muxer.hpp"

extern "C" {
#include <libavformat/avformat.h>
}
#include <boost/coroutine2/all.hpp>

typedef boost::coroutines2::coroutine<std::pair<int, AVFrame*>> FrameGen;

enum Frame { INIT = -3, END = -2, ERROR = -1 };

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

    FrameGen::pull_type m_frames;

    int openInput();
    void frame(FrameGen::push_type& yield);

   public:
    Demuxer(const char* _pFn, AVDictionary* _pOpts);
    ~Demuxer();
    inline FrameGen::pull_type& Frame() { return m_frames; };
};
