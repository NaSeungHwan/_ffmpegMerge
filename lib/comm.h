//
// Created by NAVER on 2017. 8. 29..
//

#ifndef MERGE_IN_C_COMM_H
#define MERGE_IN_C_COMM_H

#include "libavformat/avformat.h"

enum responseCode {
    SUCCESS = 0,
    OPENERR = 22,
    ALLOCERR = 23,
    READERR = 24,
    VSTREAM = 25,
    UNKNOWNERR = 20,
};

typedef struct _STREAM{
    AVOutputFormat *outputFormat;
    AVFormatContext *outputFormatContext, *inputFormatContext;
    AVPacket pkt;
    AVRational tb_base;
}stream;

/**
 * @param vid Request VID
 *
 * @return char* outputSource if return other on failure.
 */
char* createOutputSource(const char *);

#endif //MERGE_IN_C_COMM_H
