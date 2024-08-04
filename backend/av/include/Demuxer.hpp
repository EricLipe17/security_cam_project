#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <string>
#include <vector>

class Demuxer {
   private:
    AVFormatContext* m_pInFmtCtx;
    std::vector<AVCodecContext*> m_vDecCodecCtxs;
    std::vector<AVFrame*> m_vDecFrames;
    AVDictionary* m_pOpts;
    const char* m_pFn;
    int m_nErrCode;
    std::string m_szErrMsg;

    void openInput();

   public:
    Demuxer(const char* _pFn, AVDictionary* _pOpts);
    ~Demuxer();
    AVFrame* Frame(AVPacket* _pPacket);
};
