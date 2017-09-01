//
// Created by NAVER on 2017. 8. 28..
//

#ifndef MERGE_IN_C_FFMPFUNCTHION_H
#define MERGE_IN_C_FFMPFUNCTHION_H

#include "lib/comm.h"


static void _initStream(stream *);

/**
 * Open target source.
 *
 * @param target Stream 구조체, _initStream() 함수로 초기화가 필요합니다.
 *        lib/comm.h 에 구현됨
 *
 * @param source targetSource,
 *
 * @return 0 if OK
 **/
static int openInputSource(stream *, const char* );

static int initOutputSource(stream *, const char *, const char *);
static int mergeSource(stream *, const char *[]);
static void _destroyStream(stream *);
int mergeMain(const char *);

#endif //MERGE_IN_C_FFMPFUNCTHION_H
