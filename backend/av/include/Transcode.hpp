#pragma once

#include "Stream.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

typedef struct StreamingParams
{
    char m_nRemuxVideo;
    char m_nRemuxAudio;
    char* m_pOutExt;
    char* m_pFN;
    char* m_pFQN;
    char* m_pFQP;
    char* m_pMuxerOptKey;
    char* m_pMuxerOptVal;
    enum AVCodecID m_nVideoID;
    enum AVCodecID m_nAudioID;
    char* m_pCodecPrivKey;
    char* m_pCodecPrivVal;
} StreamingParams;

typedef struct StreamContext
{
    AVFormatContext* m_pFormatCtx;
    const AVCodec* m_pVideoCodec;
    const AVCodec* m_pAudioCodec;
    AVStream* m_pVideoStream;
    AVStream* m_pAudioStream;
    AVCodecContext* m_pVideoCodecCtx;
    AVCodecContext* m_pAudioCodecCtx;
    int m_nVideoIndex;
    int m_nAudioIndex;
} StreamContext;

class TranscodeContext
{
    private:
    StreamContext* m_pEncoderCtx;
    StreamContext* m_pDecoderCtx;
    StreamingParams* m_pOutParams;
    AVDictionary* m_pMuxer_Opts;
    AVRational m_nSrcFrameRate;

    int transcode_audio(AVPacket* _pInputPkt, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_);
    int transcode_video(AVPacket* _pInputPkt, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_);

    public:
    TranscodeContext(StreamingParams* _pParams, AVFormatContext* _pDecodeCtx, char* _pFN);
    ~TranscodeContext();
    int rollover_split();
    AVFormatContext* GetDecoderFmtCtx();
    AVRational GetSrcFrameRate();
    AVDictionary** GetMuxerOpts();
    int init_output_filename();
    int init_video_encoder(StreamContext* _pCtx, AVCodecContext* _pDecoderCodecCtx, AVRational framerate, StreamingParams params);
    int init_audio_encoder(StreamContext* _pCtx, int sample_rate, StreamingParams params);
    int fill_stream_info(AVStream* _pStream, const AVCodec** _ppCodec, AVCodecContext** _ppCodecCtx);
    int get_stream_refs(StreamContext* _pInCtx);
    int init_decoder_stream_ctx(StreamContext** _ppInCtx, const AVFormatContext* _pDecodeCtx);
    int init_default_params(StreamingParams** _ppParams);
    int init_context(StreamingParams* _pParams);
    int encode_video(StreamContext* _pDecodeCtx, StreamContext* _pEncodeCtx, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_);
    int encode_audio(StreamContext* _pDecodeCtx, StreamContext* _pEncodeCtx, AVFrame* _pInputFrame, AVPacket** _ppOutPkt_);
    int remux(AVPacket** _ppInputPkt, const char _nVideo, AVPacket** _ppOutputPkt_);
};





