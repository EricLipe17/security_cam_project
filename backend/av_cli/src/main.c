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
    TranscodeContext* pTranscoder = alloc_transcoder();
    StreamParams inParams;
    StreamParams outParams;
    inParams.m_pFQN = "xmas.mp4";
    outParams.m_pFQN = "xmas_t.mp4";

    init_transcoder(pTranscoder, &inParams, &outParams);
    int ret;
    do {
        ret = write_frame(pTranscoder, 0);
    } while (ret >= 0);

    for (int i = 0; i < pTranscoder->m_pDecodeCtx->m_pFmtCtx->nb_streams; ++i) {
        flush_encoder(pTranscoder, i);
    }

    write_trailer(pTranscoder);

    free_transcoder(pTranscoder);

    return 0;
}

