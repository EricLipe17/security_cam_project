#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <string>
#include <vector>

class FrameBuffer {
   private:
    std::vector<AVFrame*> m_vFrames;
    unsigned int m_nBufPtr;

   public:
    FrameBuffer() : m_nBufPtr{0}, m_vFrames{av_frame_alloc()} {}

    AVFrame* operator[](unsigned int _nIndex) {
        AVFrame* pFrame = nullptrptr;
        if (_nIndex < m_vFrames.size()) pFrame = m_vFrames[_nIndex];
        return pFrame;
    }

    std::size_t Size() { return m_vFrames.size(); }
    void PushBack(AVFrame* _pFrame) { m_vFrames.push_back(_pFrame); }
    const unsigned int GetPtr() { return m_nBufPtr; }
    void AllocFrame() {
        if (m_vFrames.size() == m_nBufPtr) {
            m_vFrames.push_back(av_frame_alloc());
        }
    }
    void IncrementPtr() { ++m_nBufPtr; }
    void DecrementPtr() { --m_nBufPtr; }
    AVFrame* GetNextFreeFrame() { return m_vFrames.at(m_nBufPtr); }
};
