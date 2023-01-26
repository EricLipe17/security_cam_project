#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
#include <libavutil/opt.h>
#include <string.h>

#include "Transcode.h"

int fill_stream_info2(AVStream *avs, AVCodec **avc, AVCodecContext **avcc) {
    *avc = avcodec_find_decoder(avs->codecpar->codec_id);
    if (!*avc) {av_log(NULL, AV_LOG_ERROR, "failed to find the codec"); return -1;}

    *avcc = avcodec_alloc_context3(*avc);
    if (!*avcc) {av_log(NULL, AV_LOG_ERROR, "failed to alloc memory for codec context"); return -1;}

    if (avcodec_parameters_to_context(*avcc, avs->codecpar) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to fill codec context"); return -1;}

    if (avcodec_open2(*avcc, *avc, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to open codec"); return -1;}
    return 0;
}

int main(int argc, char *argv[])
{
    StreamParams inParams;
    StreamParams outParams;
    inParams.m_pFQN = "xmas.mp4";
    outParams.m_pFQN = "xmas_t.m3u8";
    AVDictionary* pEncoderOpts = NULL;
    int nFlags = AV_CODEC_FLAG_CLOSED_GOP;
    int nFlags2 = 0;
    av_dict_set_int(&pEncoderOpts, "g", 30, 0);
    av_dict_set_int(&pEncoderOpts, "hls_time", 10, 0);

    TranscodeContext* pTranscoder = alloc_transcoder();
    set_encode_options(pTranscoder, pEncoderOpts);
    set_encode_flags(pTranscoder, nFlags, nFlags2);
    set_video_encode_id(pTranscoder, AV_CODEC_ID_H264);
    set_audio_encode_id(pTranscoder, AV_CODEC_ID_AAC);
    init_transcoder(pTranscoder, &inParams, &outParams);

    int ret;
    do {
        ret = write_frame(pTranscoder);
    } while (ret >= 0 || ret == AVERROR(EAGAIN));

    for (int i = 0; i < pTranscoder->m_pDecodeCtx->m_pFmtCtx->nb_streams; ++i) {
        ret = flush_encoder(pTranscoder, i);
        av_log(NULL, AV_LOG_INFO, "Info: %s\n", av_err2str(ret));
    }

    ret = write_trailer(pTranscoder);
    av_log(NULL, AV_LOG_INFO, "Info: %s\n", av_err2str(ret));

    free_transcoder(pTranscoder);

    return 0;
}

