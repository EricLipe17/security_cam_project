#include "StreamWriter.h"

#include "AVUtil.h"

#include <math.h>

//// Private



////

StreamWriter* alloc_writer()
{
    StreamWriter* pSW = malloc(sizeof(StreamWriter));
    pSW->m_pCtx = NULL;
    return pSW;
}

void free_writer(StreamWriter* _pSW)
{
    free(_pSW);
}

int init_writer(StreamWriter* _pSW, TranscodeContext* _pCtx, int _nRolloverTime)
{
    _pSW->m_pCtx = _pCtx;
    _pSW->m_nDesiredNumFrames = _nRolloverTime * ceil((double)_pCtx->m_nSrcFrameRate.num / _pCtx->m_nSrcFrameRate.den);
    _pSW->m_nCurrNumFrames = 0;
    return 0;
}

int write_header(StreamWriter* _pSW)
{
    int nResponse = avformat_write_header(_pSW->m_pCtx->m_pDecoderCtx->m_pFormatCtx, &_pSW->m_pCtx->m_pMuxer_Opts);
    return nResponse;
}

int write_trailer(StreamWriter* _pSW)
{
    int nResponse = av_write_trailer(_pSW->m_pCtx->m_pDecoderCtx->m_pFormatCtx);
    return nResponse;
}

int write_packet(StreamWriter* _pSW, AVPacket** _ppPkt)
{
    int nResponse;
    if (_pSW->m_nCurrNumFrames >= _pSW->m_nDesiredNumFrames)
        nResponse = rollover_split(_pSW->m_pCtx);

    nResponse = av_interleaved_write_frame(_pSW->m_pCtx->m_pDecoderCtx->m_pFormatCtx, *_ppPkt);
    return nResponse;
}
