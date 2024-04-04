#include "StreamWriter.h"

#include "AVUtil.h"

#include <math.h>

StreamWriter::StreamWriter(TranscodeContext* _pCtx, int _nRolloverTime)
{
    m_pCtx = _pCtx;
    AVRational srcFrameRate = _pCtx->GetSrcFrameRate();
    m_nDesiredNumFrames = _nRolloverTime * ceil((double)srcFrameRate.num / srcFrameRate.den);
    m_nCurrNumFrames = 0;
}

StreamWriter::~StreamWriter() {}

int StreamWriter::writeHeader()
{
    int nResponse = avformat_write_header(m_pCtx->GetDecoderFmtCtx(), m_pCtx->GetMuxerOpts());
    return nResponse;
}

int StreamWriter::writeTrailer()
{
    int nResponse = av_write_trailer(m_pCtx->GetDecoderFmtCtx());
    return nResponse;
}

int StreamWriter::WritePacket(AVPacket** _ppPkt)
{
    int nResponse;
    if (m_nCurrNumFrames >= m_nDesiredNumFrames)
        nResponse = m_pCtx->rollover_split();

    nResponse = av_interleaved_write_frame(m_pCtx->GetDecoderFmtCtx(), *_ppPkt);
    return nResponse;
}
