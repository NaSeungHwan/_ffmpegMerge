//
// Created by NAVER on 2017. 8. 29..
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "comm.h"

struct tm const *createTime(struct tm const *_time)
{
    time_t current_time;

    time(&current_time);
    _time=localtime(&current_time);

    return _time;
}

char *createOutputSource(const char *vid)
{
    time_t current_time;
    struct tm *time_tm;
    char path[] = "/Users/naver/Desktop/";
    char *outputSource;

    outputSource = (char*)malloc(64);
    if( !(char*)memset(outputSource, 0, 64) )
    {
        fprintf(stderr, "mem allocating error\n");
        exit(ALLOCERR);
    }

    time(&current_time);
    time_tm=localtime(&current_time);



    sprintf(outputSource, "%s%04d%02d%02d_%s", path, time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, vid);

    return outputSource;
}

void logJob(const int responseCode)
{
    struct tm const *_time = NULL;
    _time = createTime(_time);

    char path[] = "/Users/naver/Desktop/";
    //char *logFile =
    switch(responseCode)
    {
        case SUCCESS:
            fprintf(stderr, "Success, %d\n", responseCode);
            break;
        case OPENERR:
            fprintf(stderr, "file open error, %d\n", responseCode);
            break;
        case ALLOCERR:
            fprintf(stderr, "Unknown error, %d\n", responseCode);
            break;
        case READERR:
            fprintf(stderr, "Unknown error, %d\n", responseCode);
            break;
        case VSTREAM:
            fprintf(stderr, "Unknown error, %d\n", responseCode);
            break;
        default:
            fprintf(stderr, "Unknown error, %d\n", responseCode);
            break;
    }
}
