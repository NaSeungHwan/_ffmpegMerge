//
// Created by NAVER on 2017. 8. 29..
//

#ifndef MERGE_IN_C_COMM_H
#define MERGE_IN_C_COMM_H

#include "libavformat/avformat.h"

#define __toString(x) #x

enum responseCode {
    SUCCESS = 0,
    OPENERR = -2,
    ALLOCERR = -3,
    READERR = -4,
    VSTREAM = -5,
    UNKNOWNERR = 20,
    count = 6
};

typedef struct _STREAM{
    AVOutputFormat *outputFormat;
    AVFormatContext *outputFormatContext, *inputFormatContext;
    AVPacket pkt;
    AVRational tb_base;
}stream;

struct tm const *createTime(struct tm const *);

/**
 * @param vid Request VID
 *
 * @return char* outputSource if return other on failure.
 */
char *createOutputSource(const char *);

/**
 * @param responseCode enum variable in comm.h
 *
 * @print response code
 *
 * @retrun NONE
 * */
void logJob(const char *, int );

int setInputSource(const char *, char [][96]);

#endif //MERGE_IN_C_COMM_H
