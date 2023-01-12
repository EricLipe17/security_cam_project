#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
#include <stdlib.h>
#include <libavutil/opt.h>
#include <string.h>

#include "Transcode.h"
#include "StreamWriter.h"

//typedef struct StreamingParams {
//    char m_nRemuxVideo;
//    char m_nRemuxAudio;
//    char *m_pOutExt;
//    char *m_pMuxerOptKey;
//    char *m_pMuxerOptVal;
//    char *video_codec;
//    char *audio_codec;
//    char *m_pCodecPrivKey;
//    char *m_pCodecPrivVal;
//} StreamingParams;
//
//typedef struct TranscodeContext {
//    AVFormatContext *m_pFormatCtx;
//    AVCodec *m_pVideoCodec;
//    AVCodec *m_pAudioCode;
//    AVStream *video_avs;
//    AVStream *audio_avs;
//    AVCodecContext *video_avcc;
//    AVCodecContext *audio_avcc;
//    int video_index;
//    int audio_index;
//    char *filename;
//} TranscodeContext;
//
int fill_stream_info2(AVStream *avs, AVCodec **avc, AVCodecContext **avcc) {
    *avc = avcodec_find_decoder(avs->codecpar->codec_id);
    if (!*avc) {av_log(NULL, AV_LOG_ERROR, "failed to find the codec"); return -1;}

    *avcc = avcodec_alloc_context3(*avc);
    if (!*avcc) {av_log(NULL, AV_LOG_ERROR, "failed to alloc memory for codec context"); return -1;}

    if (avcodec_parameters_to_context(*avcc, avs->codecpar) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to fill codec context"); return -1;}

    if (avcodec_open2(*avcc, *avc, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to open codec"); return -1;}
    return 0;
}

int open_media2(const char *in_filename, AVFormatContext **avfc) {
    *avfc = avformat_alloc_context();
    if (!*avfc) {av_log(NULL, AV_LOG_ERROR, "failed to alloc memory for format"); return -1;}

    if (avformat_open_input(avfc, in_filename, NULL, NULL) != 0) {av_log(NULL, AV_LOG_ERROR, "failed to open input file %s", in_filename); return -1;}

    if (avformat_find_stream_info(*avfc, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "failed to get stream info"); return -1;}
    return 0;
}

int prepare_decoder2(StreamContext *sc) {
    for (int i = 0; i < sc->m_pFormatCtx->nb_streams; i++)
    {
        if (sc->m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            sc->m_pVideoStream = sc->m_pFormatCtx->streams[i];
            sc->m_nVideoIndex = i;

            if (fill_stream_info2(sc->m_pVideoStream, &sc->m_pVideoCodec, &sc->m_pVideoCodecCtx)) {return -1;}
        } else if (sc->m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            sc->m_pAudioStream = sc->m_pFormatCtx->streams[i];
            sc->m_nAudioIndex = i;

            if (fill_stream_info2(sc->m_pAudioStream, &sc->m_pAudioCodec, &sc->m_pAudioCodecCtx)) {return -1;}
        } else {
            av_log(NULL, AV_LOG_ERROR, "skipping streams other than audio and video");
        }
    }

    return 0;
}
//
//int prepare_video_encoder(TranscodeContext *sc, AVCodecContext *decoder_ctx, AVRational input_framerate, StreamingParams sp) {
//    sc->video_avs = avformat_new_stream(sc->m_pFormatCtx, NULL);
//
//    sc->m_pVideoCodec = avcodec_find_encoder_by_name(sp.video_codec);
//    if (!sc->m_pVideoCodec) {av_log(NULL, AV_LOG_ERROR, "could not find the proper codec"); return -1;}
//
//    sc->video_avcc = avcodec_alloc_context3(sc->m_pVideoCodec);
//    if (!sc->video_avcc) {av_log(NULL, AV_LOG_ERROR, "could not allocated memory for codec context"); return -1;}
//
//    av_opt_set(sc->video_avcc->priv_data, "preset", "fast", 0);
//    if (sp.m_pCodecPrivKey && sp.m_pCodecPrivVal)
//        av_opt_set(sc->video_avcc->priv_data, sp.m_pCodecPrivKey, sp.m_pCodecPrivVal, 0);
//
//    sc->video_avcc->height = decoder_ctx->height;
//    sc->video_avcc->width = decoder_ctx->width;
//    sc->video_avcc->sample_aspect_ratio = decoder_ctx->sample_aspect_ratio;
//    if (sc->m_pVideoCodec->pix_fmts)
//        sc->video_avcc->pix_fmt = sc->m_pVideoCodec->pix_fmts[0];
//    else
//        sc->video_avcc->pix_fmt = decoder_ctx->pix_fmt;
//
//    sc->video_avcc->bit_rate = 2 * 1000 * 1000;
//    sc->video_avcc->rc_buffer_size = 4 * 1000 * 1000;
//    sc->video_avcc->rc_max_rate = 2 * 1000 * 1000;
//    sc->video_avcc->rc_min_rate = 2.5 * 1000 * 1000;
//
//    sc->video_avcc->time_base = av_inv_q(input_framerate);
//    sc->video_avs->time_base = sc->video_avcc->time_base;
//
//    if (avcodec_open2(sc->video_avcc, sc->m_pVideoCodec, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "could not open the codec"); return -1;}
//    avcodec_parameters_from_context(sc->video_avs->codecpar, sc->video_avcc);
//    return 0;
//}
//
//int prepare_audio_encoder(TranscodeContext *sc, int sample_rate, StreamingParams sp){
//    sc->audio_avs = avformat_new_stream(sc->m_pFormatCtx, NULL);
//
//    sc->m_pAudioCode = avcodec_find_encoder_by_name(sp.audio_codec);
//    if (!sc->m_pAudioCode) {av_log(NULL, AV_LOG_ERROR, "could not find the proper codec"); return -1;}
//
//    sc->audio_avcc = avcodec_alloc_context3(sc->m_pAudioCode);
//    if (!sc->audio_avcc) {av_log(NULL, AV_LOG_ERROR, "could not allocated memory for codec context"); return -1;}
//
//    int OUTPUT_CHANNELS = 2;
//    int OUTPUT_BIT_RATE = 196000;
//    sc->audio_avcc->channels       = OUTPUT_CHANNELS;
//    sc->audio_avcc->channel_layout = av_get_default_channel_layout(OUTPUT_CHANNELS);
//    sc->audio_avcc->sample_rate    = sample_rate;
//    sc->audio_avcc->sample_fmt     = sc->m_pAudioCode->sample_fmts[0];
//    sc->audio_avcc->bit_rate       = OUTPUT_BIT_RATE;
//    sc->audio_avcc->time_base      = (AVRational){1, sample_rate};
//
//    sc->audio_avcc->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
//
//    sc->audio_avs->time_base = sc->audio_avcc->time_base;
//
//    if (avcodec_open2(sc->audio_avcc, sc->m_pAudioCode, NULL) < 0) {av_log(NULL, AV_LOG_ERROR, "could not open the codec"); return -1;}
//    avcodec_parameters_from_context(sc->audio_avs->codecpar, sc->audio_avcc);
//    return 0;
//}
//
//int prepare_copy(AVFormatContext *avfc, AVStream **avs, AVCodecParameters *decoder_par) {
//    *avs = avformat_new_stream(avfc, NULL);
//    avcodec_parameters_copy((*avs)->codecpar, decoder_par);
//    return 0;
//}
//
//int remux(AVPacket **pkt, AVFormatContext **avfc, AVRational decoder_tb, AVRational encoder_tb) {
//    av_packet_rescale_ts(*pkt, decoder_tb, encoder_tb);
//    if (av_interleaved_write_frame(*avfc, *pkt) < 0) { av_log(NULL, AV_LOG_ERROR, "error while copying stream packet"); return -1; }
//    return 0;
//}
//
//int encode_video(TranscodeContext *decoder, TranscodeContext *encoder, AVFrame *input_frame) {
//    if (input_frame) input_frame->pict_type = AV_PICTURE_TYPE_NONE;
//
//    AVPacket *output_packet = av_packet_alloc();
//    if (!output_packet) {av_log(NULL, AV_LOG_ERROR, "could not allocate memory for output packet"); return -1;}
//
//    int response = avcodec_send_frame(encoder->video_avcc, input_frame);
//
//    while (response >= 0) {
//        response = avcodec_receive_packet(encoder->video_avcc, output_packet);
//        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
//            break;
//        } else if (response < 0) {
//            av_log(NULL, AV_LOG_ERROR, "Error while receiving packet from encoder: %s", av_err2str(response));
//            return -1;
//        }
//
//        output_packet->stream_index = decoder->video_index;
//        output_packet->duration = encoder->video_avs->time_base.den / encoder->video_avs->time_base.num / decoder->video_avs->avg_frame_rate.num * decoder->video_avs->avg_frame_rate.den;
//
//        av_packet_rescale_ts(output_packet, decoder->video_avs->time_base, encoder->video_avs->time_base);
//        response = av_interleaved_write_frame(encoder->m_pFormatCtx, output_packet);
//        if (response != 0) { av_log(NULL, AV_LOG_ERROR, "Error %d while receiving packet from decoder: %s", response, av_err2str(response)); return -1;}
//    }
//    av_packet_unref(output_packet);
//    av_packet_free(&output_packet);
//    return 0;
//}
//
//int encode_audio(TranscodeContext *decoder, TranscodeContext *encoder, AVFrame *input_frame) {
//    AVPacket *output_packet = av_packet_alloc();
//    if (!output_packet) {av_log(NULL, AV_LOG_ERROR, "could not allocate memory for output packet"); return -1;}
//
//    int response = avcodec_send_frame(encoder->audio_avcc, input_frame);
//
//    while (response >= 0) {
//        response = avcodec_receive_packet(encoder->audio_avcc, output_packet);
//        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
//            break;
//        } else if (response < 0) {
//            av_log(NULL, AV_LOG_ERROR, "Error while receiving packet from encoder: %s", av_err2str(response));
//            return -1;
//        }
//
//        output_packet->stream_index = decoder->audio_index;
//
//        av_packet_rescale_ts(output_packet, decoder->audio_avs->time_base, encoder->audio_avs->time_base);
//        response = av_interleaved_write_frame(encoder->m_pFormatCtx, output_packet);
//        if (response != 0) { av_log(NULL, AV_LOG_ERROR, "Error %d while receiving packet from decoder: %s", response, av_err2str(response)); return -1;}
//    }
//    av_packet_unref(output_packet);
//    av_packet_free(&output_packet);
//    return 0;
//}
//
//int transcode_audio(TranscodeContext *decoder, TranscodeContext *encoder, AVPacket *input_packet, AVFrame *input_frame) {
//    int response = avcodec_send_packet(decoder->audio_avcc, input_packet);
//    if (response < 0) {av_log(NULL, AV_LOG_ERROR, "Error while sending packet to decoder: %s", av_err2str(response)); return response;}
//
//    while (response >= 0) {
//        response = avcodec_receive_frame(decoder->audio_avcc, input_frame);
//        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
//            break;
//        } else if (response < 0) {
//            av_log(NULL, AV_LOG_ERROR, "Error while receiving frame from decoder: %s", av_err2str(response));
//            return response;
//        }
//
//        if (response >= 0) {
//            if (encode_audio(decoder, encoder, input_frame)) return -1;
//        }
//        av_frame_unref(input_frame);
//    }
//    return 0;
//}
//
//int transcode_video(TranscodeContext *decoder, TranscodeContext *encoder, AVPacket *input_packet, AVFrame *input_frame) {
//    int response = avcodec_send_packet(decoder->video_avcc, input_packet);
//    if (response < 0) {av_log(NULL, AV_LOG_ERROR, "Error while sending packet to decoder: %s", av_err2str(response)); return response;}
//
//    while (response >= 0) {
//        response = avcodec_receive_frame(decoder->video_avcc, input_frame);
//        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
//            break;
//        } else if (response < 0) {
//            av_log(NULL, AV_LOG_ERROR, "Error while receiving frame from decoder: %s", av_err2str(response));
//            return response;
//        }
//
//        if (response >= 0) {
//            if (encode_video(decoder, encoder, input_frame)) return -1;
//        }
//        av_frame_unref(input_frame);
//    }
//    return 0;
//}

int main(int argc, char *argv[])
{
    /*
     * H264 -> H265
     * Audio -> remuxed (untouched)
     * MP4 - MP4
     */
    StreamingParams sp = {0};
    sp.m_nRemuxAudio = 1;
    sp.m_nRemuxVideo = 0;
    sp.m_nVideoID = AV_CODEC_ID_HEVC;
    sp.m_pCodecPrivKey = "x265-params";
    sp.m_pCodecPrivVal = "keyint=60:min-keyint=60:scenecut=0";
    sp.m_nAudioID = AV_CODEC_ID_AAC;
    sp.m_pOutExt = "mp4";

    /*
     * H264 -> H264 (fixed gop)
     * Audio -> remuxed (untouched)
     * MP4 - MP4
     */
    //StreamingParams sp = {0};
    //sp.m_nRemuxAudio = 1;
    //sp.m_nRemuxVideo = 0;
    //sp.video_codec = "libx264";
    //sp.m_pCodecPrivKey = "x264-params";
    //sp.m_pCodecPrivVal = "keyint=60:min-keyint=60:scenecut=0:force-cfr=1";

    /*
     * H264 -> H264 (fixed gop)
     * Audio -> remuxed (untouched)
     * MP4 - fragmented MP4
     */
    //StreamingParams sp = {0};
    //sp.m_nRemuxAudio = 1;
    //sp.m_nRemuxVideo = 0;
    //sp.video_codec = "libx264";
    //sp.m_pCodecPrivKey = "x264-params";
    //sp.m_pCodecPrivVal = "keyint=60:min-keyint=60:scenecut=0:force-cfr=1";
    //sp.m_pMuxerOptKey = "movflags";
    //sp.m_pMuxerOptVal = "frag_keyframe+empty_moov+delay_moov+default_base_moof";

    /*
     * H264 -> H264 (fixed gop)
     * Audio -> AAC
     * MP4 - MPEG-TS
     */
    //StreamingParams sp = {0};
    //sp.m_nRemuxAudio = 0;
    //sp.m_nRemuxVideo = 0;
    //sp.video_codec = "libx264";
    //sp.m_pCodecPrivKey = "x264-params";
    //sp.m_pCodecPrivVal = "keyint=60:min-keyint=60:scenecut=0:force-cfr=1";
    //sp.audio_codec = "aac";
    //sp.m_pOutExt = ".ts";

    /*
     * H264 -> VP9
     * Audio -> Vorbis
     * MP4 - WebM
     */
    //StreamingParams sp = {0};
    //sp.m_nRemuxAudio = 0;
    //sp.m_nRemuxVideo = 0;
    //sp.video_codec = "libvpx-vp9";
    //sp.audio_codec = "libvorbis";
    //sp.m_pOutExt = ".webm";

    StreamContext *decoder = (StreamContext*) calloc(1, sizeof(StreamContext));
    const char* pFN = argv[1];

//
//    TranscodeContext *encoder = (TranscodeContext*) calloc(1, sizeof(TranscodeContext));
//    encoder->filename = argv[2];
//
//    if (sp.m_pOutExt)
//        strcat(encoder->filename, sp.m_pOutExt);
//
    if (open_media2(pFN, &decoder->m_pFormatCtx)) return -1;
    if (prepare_decoder2(decoder)) return -1;
//
//    avformat_alloc_output_context2(&encoder->m_pFormatCtx, NULL, NULL, encoder->filename);
//    if (!encoder->m_pFormatCtx) {av_log(NULL, AV_LOG_ERROR, "could not allocate memory for output format");return -1;}
//
//    if (!sp.m_nRemuxVideo) {
//        AVRational input_framerate = av_guess_frame_rate(decoder->m_pFormatCtx, decoder->video_avs, NULL);
//        prepare_video_encoder(encoder, decoder->video_avcc, input_framerate, sp);
//    } else {
//        if (prepare_copy(encoder->m_pFormatCtx, &encoder->video_avs, decoder->video_avs->codecpar)) {return -1;}
//    }
//
//    if (!sp.m_nRemuxAudio) {
//        if (prepare_audio_encoder(encoder, decoder->audio_avcc->sample_rate, sp)) {return -1;}
//    } else {
//        if (prepare_copy(encoder->m_pFormatCtx, &encoder->audio_avs, decoder->audio_avs->codecpar)) {return -1;}
//    }
//
//    if (encoder->m_pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER)
//        encoder->m_pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//
//    if (!(encoder->m_pFormatCtx->oformat->flags & AVFMT_NOFILE)) {
//        if (avio_open(&encoder->m_pFormatCtx->pb, encoder->filename, AVIO_FLAG_WRITE) < 0) {
//            av_log(NULL, AV_LOG_ERROR, "could not open the output file");
//            return -1;
//        }
//    }
//
//    AVDictionary* muxer_opts = NULL;
//
//    if (sp.m_pMuxerOptKey && sp.m_pMuxerOptVal) {
//        av_dict_set(&muxer_opts, sp.m_pMuxerOptKey, sp.m_pMuxerOptVal, 0);
//    }
//
//    if (avformat_write_header(encoder->m_pFormatCtx, &muxer_opts) < 0) {av_log(NULL, AV_LOG_ERROR, "an error occurred when opening output file"); return -1;}

    TranscodeContext* pTranscoder = alloc_transcoder();
    init_transcoder(pTranscoder, &sp, decoder->m_pFormatCtx, "output");

    StreamWriter* pSW = alloc_writer();
    init_writer(pSW, pTranscoder, 10000);
    write_header(pSW);

    AVFrame *input_frame = av_frame_alloc();
    if (!input_frame) {av_log(NULL, AV_LOG_ERROR, "failed to allocated memory for AVFrame"); return -1;}

    AVPacket *input_packet = av_packet_alloc();
    if (!input_packet) {av_log(NULL, AV_LOG_ERROR, "failed to allocated memory for AVPacket"); return -1;}
    AVPacket *output_packet = av_packet_alloc();

    while (av_read_frame(decoder->m_pFormatCtx, input_packet) >= 0)
    {
        if (decoder->m_pFormatCtx->streams[input_packet->stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            transcode_video(pTranscoder, input_packet, input_frame, &output_packet);
            write_packet(pSW, &output_packet);
        }
        else if (decoder->m_pFormatCtx->streams[input_packet->stream_index]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            transcode_audio(pTranscoder, input_packet, input_frame, &output_packet);
            write_packet(pSW, &output_packet);
        }
        else
        {
            av_log(NULL, AV_LOG_ERROR, "ignoring all non video or audio packets");
        }
    }

    write_trailer(pSW);
    free_writer(pSW);

    free_transcoder(pTranscoder);

//    // TODO: should I also flush the audio encoder?
//    if (encode_video(decoder, encoder, NULL)) return -1;
//
//    av_write_trailer(encoder->m_pFormatCtx);
//
//    if (muxer_opts != NULL) {
//        av_dict_free(&muxer_opts);
//        muxer_opts = NULL;
//    }
//
//    if (input_frame != NULL) {
//        av_frame_free(&input_frame);
//        input_frame = NULL;
//    }
//
//    if (input_packet != NULL) {
//        av_packet_free(&input_packet);
//        input_packet = NULL;
//    }
//
//    avformat_close_input(&decoder->m_pFormatCtx);
//
//    avformat_free_context(decoder->m_pFormatCtx); decoder->m_pFormatCtx = NULL;
//    avformat_free_context(encoder->m_pFormatCtx); encoder->m_pFormatCtx = NULL;
//
//    avcodec_free_context(&decoder->video_avcc); decoder->video_avcc = NULL;
//    avcodec_free_context(&decoder->audio_avcc); decoder->audio_avcc = NULL;
//
//    free(decoder); decoder = NULL;
//    free(encoder); encoder = NULL;

    return 0;
}

