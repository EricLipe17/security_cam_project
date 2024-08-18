#include <iostream>
#include "AVUtil.hpp"
#include "Demuxer.hpp"
#include "Muxer.hpp"


int main(int argc, char** argv) {
    Demuxer demux("/home/eric/Downloads/sample_1280x720_surfing_with_audio.mp4", nullptr);
    Muxer mux(demux.m_pFmtCtx, demux.m_vCodecCtxs, "sample_1280x720_surfing_with_audio_new.m3u8",
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

    // if ((ret = open_input_file("/home/eric/Downloads/sample_1280x720_surfing_with_audio.mp4")) <
    // 0)
    //     exit(1);
    // if ((ret = open_output_file("sample_1280x720_surfing_with_audio_new.m3u8")) < 0) exit(1);
    // if ((ret = init_filters()) < 0) exit(1);
    // if (!(packet = av_packet_alloc())) exit(1);

    // /* read all packets */
    // while (1) {
    //     if ((ret = av_read_frame(ifmt_ctx, packet)) < 0) break;
    //     stream_index = packet->stream_index;
    //     av_log(nullptr, AV_LOG_DEBUG, "Demuxer gave frame of stream_index %u\n", stream_index);

    //     if (filter_ctx[stream_index].filter_graph) {
    //         StreamContext* stream = &stream_ctx[stream_index];

    //         av_log(nullptr, AV_LOG_DEBUG, "Going to reencode&filter the frame\n");

    //         av_packet_rescale_ts(packet, ifmt_ctx->streams[stream_index]->time_base,
    //                              stream->dec_ctx->time_base);
    //         ret = avcodec_send_packet(stream->dec_ctx, packet);
    //         if (ret < 0) {
    //             av_log(nullptr, AV_LOG_ERROR, "Decoding failed\n");
    //             break;
    //         }

    //         while (ret >= 0) {
    //             ret = avcodec_receive_frame(stream->dec_ctx, stream->dec_frame);
    //             if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
    //                 break;
    //             else if (ret < 0)
    //                 exit(1);

    //             stream->dec_frame->pts = stream->dec_frame->best_effort_timestamp;
    //             ret = filter_encode_write_frame(stream->dec_frame, stream_index);
    //             if (ret < 0) exit(1);
    //         }
    //     } else {
    //         /* remux this frame without reencoding */
    //         av_packet_rescale_ts(packet, ifmt_ctx->streams[stream_index]->time_base,
    //                              ofmt_ctx->streams[stream_index]->time_base);

    //         ret = av_interleaved_write_frame(ofmt_ctx, packet);
    //         if (ret < 0) exit(1);
    //     }
    //     av_packet_unref(packet);
    // }

    // /* flush filters and encoders */
    // for (i = 0; i < ifmt_ctx->nb_streams; i++) {
    //     /* flush filter */
    //     if (!filter_ctx[i].filter_graph) continue;
    //     ret = filter_encode_write_frame(nullptr, i);
    //     if (ret < 0) {
    //         av_log(nullptr, AV_LOG_ERROR, "Flushing filter failed\n");
    //         exit(1);
    //     }

    //     /* flush encoder */
    //     ret = flush_encoder(i);
    //     if (ret < 0) {
    //         av_log(nullptr, AV_LOG_ERROR, "Flushing encoder failed\n");
    //         exit(1);
    //     }
    // }

    // av_write_trailer(ofmt_ctx);

    // av_packet_free(&packet);
    // for (i = 0; i < ifmt_ctx->nb_streams; i++) {
    //     avcodec_free_context(&stream_ctx[i].dec_ctx);
    //     if (ofmt_ctx && ofmt_ctx->nb_streams > i && ofmt_ctx->streams[i] &&
    //     stream_ctx[i].enc_ctx)
    //         avcodec_free_context(&stream_ctx[i].enc_ctx);
    //     if (filter_ctx && filter_ctx[i].filter_graph) {
    //         avfilter_graph_free(&filter_ctx[i].filter_graph);
    //         av_packet_free(&filter_ctx[i].enc_pkt);
    //         av_frame_free(&filter_ctx[i].filtered_frame);
    //     }

    //     av_frame_free(&stream_ctx[i].dec_frame);
    // }
    // av_free(filter_ctx);
    // av_free(stream_ctx);
    // avformat_close_input(&ifmt_ctx);
    // if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) avio_closep(&ofmt_ctx->pb);
    // avformat_free_context(ofmt_ctx);

    // return ret ? 1 : 0;
}
