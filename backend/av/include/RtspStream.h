#pragma once

#include <stdbool.h>
#include <vector>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
    #include <libswscale/swscale.h>
}

class RtspStream {
    private:
        const char*  m_pURL;
        AVFormatContext *m_pFormatContext;
        AVDictionary *m_pOpts;
        std::vector<AVCodecParameters*> m_vCodecParams;
        std::vector<const AVCodec*> m_vCodecs;
        std::vector<AVCodecContext*> m_vCodecContexts;
        struct SwsContext* m_pSwsScalerContext;
        unsigned int m_nNumCodecContainers;
        AVFrame* m_pVideoFrame;
        AVFrame* m_pAudioFrame;
        unsigned char* m_pFrameBuffer;
        unsigned char* m_pAudioBuffer;
        unsigned long m_nFrameBufferSize;
        int m_nHeight;
        int m_nWidth;
        int m_nVideoStreamIndex;
        int m_nAudioStreamIndex;
        char* m_pErrMsg;

        int initStream();
        int initBuffers(int _nMaxTries);
        int initFormatter(enum AVPixelFormat _fmt, int _nFlags, SwsFilter *_pSrcFilter, 
                           SwsFilter *_pDstFilter, const double *_pParam);
        int formatFrame();
        int decompress_packet(const int _nCtxIndex, AVFrame** _pFrame, AVPacket** _pPkt);

    public:
        RtspStream(const char*  _pURL);
        ~RtspStream();
        int GetNextFrame();
};
