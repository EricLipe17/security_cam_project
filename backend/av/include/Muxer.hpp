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
    AVFormatContext* m_pFmtCtx;
    std::vector<AVCodecContext*> m_vCodecCtxs;

    // TODO: Get rid of demuxer members;
    AVFormatContext* m_pDemuxerFmtCtx;
    std::vector<AVCodecContext*>& m_vDemuxerCodecCtxs;

    const std::string m_szFn;
    const std::string m_szVideoFilterSpec;
    const std::string m_szAudioFilterSpec;
    int m_nErrCode;
    std::string m_szErrMsg;

    void openOutput();
    int initFilter(FilteringContext* _pFilterCtx, AVCodecContext* _pDecCtx,
                   AVCodecContext* _pEncCtx, const std::string& _szFilterSpec);
    int filterEncodeWriteFrame(AVFrame* _pFrame, const unsigned int _nStreamIndex);
    int encodeWriteFrame(FilteringContext _filterCtx, const unsigned int _nStreamIndex,
                         const int _nFlush);
    int initFilters();

   public:
    Muxer(AVFormatContext* _pDemuxerFmtCtx, std::vector<AVCodecContext*>& _vDemuxerCodecCtxs,
          const std::string& _szFn, AVDictionary* _pOpts,
          const std::string& _szVideoFilterSpec = "null",
          const std::string& _szAudioFilterSpec = "anull");
    Muxer(Muxer&& _muxer);
    Muxer& operator=(Muxer&&) = delete;
    Muxer(Muxer&) = delete;
    Muxer& operator=(const Muxer&) = delete;
    ~Muxer();

    int WriteFrame(AVFrame* _pFrame, const unsigned int _nStreamIndex);
    int Flush();
    int CloseStream();
};
