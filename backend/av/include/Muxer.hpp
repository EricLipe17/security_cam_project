#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
}

#include "AVUtil.hpp"

struct FilteringContext {
    AVFilterContext* m_pBufferSinkCtx;
    AVFilterContext* m_pBufferSrcCtx;
    AVFilterGraph* m_pFilterGraph;

    AVPacket* enc_pkt;
    AVFrame* pFilteredFrame;
};

class Muxer {
   private:
    FilteringContext m_filterCtx;

    void openInput();
    void initFilter();
    void filterEncodeWriteFrame();
    void encodeWriteFrame();

   public:
    Muxer(const char* _pFn, AVDictionary* _pOpts);
    ~Muxer();
    void InitFilters();
    void WriteBuffer(const FrameBuffer& _demuxerBuf);
    void Flush();
};
