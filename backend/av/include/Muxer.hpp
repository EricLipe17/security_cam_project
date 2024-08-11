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

    AVPacket* m_pPacket;
    AVFrame* pFilteredFrame;
};

class Muxer {
   private:
    FilteringContext m_filterCtx;
    AVFormatContext* m_pOutFmtCtx;
    std::vector<AVCodecContext*> m_vCodecCtxs;
    AVFormatContext* m_pDemuxerFmtCtx;
    std::vector<AVCodecContext*> m_vDemuxerCodecCtxs;
    AVCodecContext* m_pEncCodecCtx;
    const char* m_pFn;
    int m_nErrCode;
    std::string m_szErrMsg;

    void openInput();
    void initFilter();
    void filterEncodeWriteFrame();
    void encodeWriteFrame();

   public:
    Muxer(AVFormatContext* _pDemuxerFmtCtx, std::vector<AVCodecContext*>& _vDemuxerCodecCtxs,
          const char* _pFn, AVDictionary* _pOpts);
    ~Muxer();
    void InitFilters();
    void WriteBuffer(const FrameBuffer& _demuxerBuf);
    void Flush();
};
