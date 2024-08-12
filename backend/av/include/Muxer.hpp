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
    AVFrame* m_pFrame;
};

class Muxer {
   private:
    std::vector<FilteringContext> m_vFilterCtxs;
    AVFormatContext* m_pOutFmtCtx;
    std::vector<AVCodecContext*> m_vCodecCtxs;
    AVFormatContext* m_pDemuxerFmtCtx;
    std::vector<AVCodecContext*>& m_vDemuxerCodecCtxs;
    AVCodecContext* m_pEncCodecCtx;
    const char* m_pFn;
    int m_nErrCode;
    std::string m_szErrMsg;

    void openOutput();
    int initFilter(FilteringContext* _pFilterCtx, AVCodecContext* _pDecCtx,
                   AVCodecContext* _pEncCtx, const char* pFilterSpec);
    int filterEncodeWriteFrame(AVFrame* _pFrame, const unsigned int _nStreamIndex);
    int encodeWriteFrame(FilteringContext _filterCtx, const unsigned int _nStreamIndex,
                         const int _nFlush);

   public:
    Muxer(AVFormatContext* _pDemuxerFmtCtx, std::vector<AVCodecContext*>& _vDemuxerCodecCtxs,
          const char* _pFn, AVDictionary* _pOpts);
    ~Muxer();
    int InitFilters(const char* _pVideoFilterSpec = NULL, const char* _pAudioFilterSpec = NULL);
    int WriteFrame(AVFrame* _pFrame, const unsigned int _nStreamIndex);
    int Flush(const unsigned int _nStreamIndex);
};
