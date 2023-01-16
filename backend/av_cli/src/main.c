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
    return 0;
}

