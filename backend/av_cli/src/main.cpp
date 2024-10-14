#include <iostream>

#include "AVUtil.hpp"
#include "Demuxer.hpp"
#include "Muxer.hpp"

int main(int argc, char** argv) {
    Demuxer demux("/home/eric/Downloads/sample_1280x720_surfing_with_audio.mp4", nullptr);
    Muxer mux(demux.m_pFmtCtx, demux.m_vCodecCtxs, "out_%Y:%m:%d %H:%M:%S.mp4",
              nullptr);
    int nErrCode = 0;
    for (auto& pair : demux.Frame()) {
        if (pair.first == Frame::INIT) continue;
        if (pair.first == Frame::END) break;
        if (pair.first == Frame::ERROR) exit(1);
        mux.WriteFrame(pair.second, pair.first);
    }
    mux.Flush();
    mux.CloseStream();

    return 0;
}
