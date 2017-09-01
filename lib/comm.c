//
// Created by NAVER on 2017. 8. 29..
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "comm.h"

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


    if(time_tm->tm_mon+1 > 10)
        sprintf(outputSource, "%s%d%d%d_%s", path, time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, vid );
    else
        sprintf(outputSource, "%s%d0%d%d_%s", path, time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, vid);

    printf("created prefix : %s\n", outputSource);

    return outputSource;
}
