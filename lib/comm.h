//
// Created by NAVER on 2017. 8. 29..
//

#ifndef MERGE_IN_C_COMM_H
#define MERGE_IN_C_COMM_H

#include "libavformat/avformat.h"


struct tm const *createTime(struct tm const *);

/**
 * @param responseCode enum variable in comm.h
 *
 * @print response code
 *
 * @retrun NONE
 * */
void logJob(const char *, const char *);

int setInputSource(const char *, char [][96]);

#endif //MERGE_IN_C_COMM_H
