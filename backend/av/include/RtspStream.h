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

        int init_stream();
        int init_buffers(int _nMaxTries);

    public:
        RtspStream(const char*  _pURL);
        ~RtspStream();

        
        int get_next_frame();
        int format_frame();
        int init_formatter(enum AVPixelFormat _fmt, int _nFlags, SwsFilter *_pSrcFilter, 
                           SwsFilter *_pDstFilter, const double *_pParam);
        int decompress_packet(const int _nCtxIndex, AVFrame** _pFrame, AVPacket** _pPkt);

};
