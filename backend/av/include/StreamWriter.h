#pragma once

#include "Transcode.h"

extern "C" {
    #include <libavformat/avformat.h>
}

class StreamWriter
{
    private:
    TranscodeContext* m_pCtx;
    unsigned int m_nDesiredNumFrames;
    unsigned int m_nCurrNumFrames;

    int WritePacket(AVPacket** _ppPkt);

    public:
    StreamWriter(TranscodeContext* _pCtx, int _nRolloverTime);
    ~StreamWriter();
    int writeHeader();
    int writeTrailer();
};
