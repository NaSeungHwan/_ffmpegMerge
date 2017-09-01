//
// Created by NAVER on 2017. 8. 28..
//
#include <stdio.h>
#include <stdlib.h>

#define __STDC_FORMAT_MACROS
#define __STDC_CONSTANT_MACROS

#include "libavformat/avformat.h"
#include "libavutil/timestamp.h"
#include "mergeFunction.h"

/** init variable **/

static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
        tag,
        av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
        av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
        av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
        pkt->stream_index);

}

static void _initStream(stream *target)
{
    target->inputFormatContext = NULL;
    target->outputFormat = NULL;
    target->outputFormatContext = NULL;
    av_register_all();
}


static int openInputSource(stream *target, const char* source)
{
    if ( (avformat_open_input(&target->inputFormatContext, source, NULL, NULL)) )
    {
        fprintf(stderr, "Could not open input source '%s'", source);
        //return _destroyStream(target,OPENERR);
        return OPENERR;
    }

    if ( (avformat_find_stream_info(target->inputFormatContext, NULL) < 0) )
    {
        fprintf(stderr, "Failed to retrieve input source info");
        //return _destroyStream(target,READERR);
        return READERR;
    }
    printf("stream : %d\n", target->inputFormatContext->nb_streams);

    //av_dump_format(target->inputFormatContext, 0, source, 0); // print input source info

    return target->inputFormatContext->nb_streams;
}

static int initOutputSource(stream *target, const char *source, const char *vid)
{
    char *sufix = "_test.mp4";
    char *outputSource;

    /* get inputSource info
     * if return < 2 on failure, return VSTREAM
     * */
    if ( (openInputSource(target, source) < 2) )
    {
        _destroyStream(target);
        return VSTREAM;
    }

    /* allocate outputSource */
    outputSource = createOutputSource(vid);
    strcat(outputSource, sufix);
    printf("output source : %s\n", outputSource);

    avformat_alloc_output_context2(&target->outputFormatContext, NULL, NULL,
                                   outputSource);
    target->outputFormat = target->outputFormatContext->oformat;
    for ( int i = 0; i< target->inputFormatContext->nb_streams; i++ )
    {
        AVStream *inStream = target->inputFormatContext->streams[i];
        /* inStream->codecpar instead of inSteam->codec->codec */
        AVStream *outStream = avformat_new_stream(target->outputFormatContext, inStream->codecpar);

        if ( !outStream )
        {
            fprintf(stderr, "Failed allocating output stream\n");
            _destroyStream(target);
            return ALLOCERR;
        }
        /*
         * avcodec_parameters_copy(outStream->codecpar, inStream->codecpar)
         * avcodec_copy_context(outStream->codec, inStream->codec);
         * avcodec_parameters_copy() instead of avcodec_copy_context()
         */

        if ( (avcodec_parameters_copy(outStream->codecpar, inStream->codecpar) < 0) )
        {
            fprintf(stderr, "Failed to copy context from input to output stream codec context\n");
            _destroyStream(target);
            return READERR;
        }

        outStream->codecpar->codec_tag = 0;
        /*
        if(target->outputFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
            outStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER; // x264 lib에 정상적으로 설정이 되는지 확인용도
        */
    }
    //av_dump_format(target->outputFormatContext, 0, outputSource, 1); // print output source info
    if (!(target->outputFormatContext->flags & AVFMT_NOFILE)) {

        if ( (avio_open(&target->outputFormatContext->pb, outputSource, AVIO_FLAG_WRITE) < 0) )
        {
            fprintf(stderr, "Could not open output file '%s'", outputSource);
            return OPENERR;
        }
    }


    if ( (avformat_write_header(target->outputFormatContext, NULL) < 0) )
    {
        fprintf(stderr, "Error occurred when opening output file\n");
        return OPENERR;
    }
    /** close input context using init outputSource **/
    avformat_close_input(&target->inputFormatContext);

    return SUCCESS;
}

int mergeSource(stream *target, const char *source[])
{
    int64_t pts_offset = 0;
    int64_t dts_offset = 0;

    target->tb_base.num = 1;
    target->tb_base.den = AV_TIME_BASE;

    int audio_count = 0;

    int file_count = sizeof(*source) / sizeof(source[0]);
    printf("source count : %d\n", file_count);
    for (int i = 0; i < file_count; i++) {


        if ( (avformat_open_input(&target->inputFormatContext, source[i], 0, 0) < 0) ) {
            fprintf(stderr, "Could not open input file '%s'", source[i]);
            _destroyStream(target);
            return OPENERR;
        }

        if ( (avformat_find_stream_info(target->inputFormatContext, 0) < 0 ) ) {
            fprintf(stderr, "Failed to retrieve input stream information");
            _destroyStream(target);
            return READERR;
        }

        int64_t last_dts = 0;
        int64_t last_pts = 0;

        while (1) {
            AVStream *in_stream, *out_stream;

            if ( ( av_read_frame(target->inputFormatContext, &target->pkt) < 0) )
                break;

            if ( target->inputFormatContext->nb_streams == 2 ) {
                in_stream = target->inputFormatContext->streams[target->pkt.stream_index];
                out_stream = target->outputFormatContext->streams[target->pkt.stream_index];
            }
            else {
                //audio stream만 있는 경우
                in_stream = target->inputFormatContext->streams[target->pkt.stream_index];
                out_stream = target->outputFormatContext->streams[1];
            }

            //log_packet(target->inputFormatContext, &target->pkt, "in");

            /* copy packet */
            target->pkt.pts = av_rescale_q_rnd(target->pkt.pts, in_stream->time_base, out_stream->time_base,
                                               AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
            target->pkt.dts = av_rescale_q_rnd(target->pkt.dts, in_stream->time_base, out_stream->time_base,
                                               AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
            target->pkt.duration = av_rescale_q(target->pkt.duration, in_stream->time_base, out_stream->time_base);
            target->pkt.pos = -1;

            //log_packet(target->outputFormatContext, &target->pkt, "out");

            int64_t conv_dts = target->pkt.dts + target->pkt.duration;

            conv_dts = av_rescale_q_rnd(conv_dts, out_stream->time_base, target->tb_base,
                                        AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
            if (conv_dts > last_dts) {
                last_dts = conv_dts;
            }

            int64_t offset = av_rescale_q_rnd(dts_offset, target->tb_base, out_stream->time_base,
                                              AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);

            target->pkt.pts += offset;
            target->pkt.dts += offset;

            //audio stream만 있는 경우 stream index를 보정해준다.
            if (target->inputFormatContext->nb_streams != 2) {
                target->pkt.stream_index = 1;
                ++audio_count;
            }
            else {
                if (target->pkt.duration < 0) {
                    int k = 0;
                }
            }

            if ( (av_interleaved_write_frame(target->outputFormatContext, &target->pkt) < 0) ) {
                fprintf(stderr, "Error muxing packet\n");
                break;
            }
            av_packet_unref(&target->pkt);
        }

        dts_offset += last_dts;

        avformat_close_input(&target->inputFormatContext);

    }

    av_write_trailer(target->outputFormatContext);
}

static void _destroyStream(stream *target)
{
    avformat_close_input(&target->inputFormatContext);
    if ( target->outputFormat != NULL && target->outputFormatContext != NULL ) {
        /* close output */
        if ( target->outputFormatContext && !(target->outputFormat->flags & AVFMT_NOFILE) )
            avio_close(target->outputFormatContext->pb);
        avformat_free_context(target->outputFormatContext);
    }
/*
    if ( ret < 0 && ret != AVERROR_EOF ) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        exit(UNKNOWNERR);
    }*/
}

/** main function **/
int mergeMain(const char *vid) {

    /* variable definition */
    int     ret=0;
    int     i=0;
    int     nonVideo = 0;

    // input source 추후, 특정 path내의 미디어 파일을 모두 찾아 넣도록 할 예정
    const char  *source[] = {"/Users/naver/Desktop/20170801_real_37493_JGAs7dKftM_M.mp4"};
    stream      target; // merge 에 필요한 변수들이 있는 struct

    /*
     * init source
     * outputSorce init용 source가 stream이 1개일시, 다음 소스로 init
     */

    while(1) {
        _initStream(&target);
        ret = initOutputSource(&target, source[i++], vid);
        if( ret != VSTREAM )
            break;
        nonVideo++;
    }

    ret = mergeSource(&target, source);

    fprintf(stderr, "mergeMain return : %d\n", ret );
    return ret;
}