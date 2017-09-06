//
// Created by NAVER on 2017. 8. 29..
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>
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
    char path[] = "/Users/naver/Desktop/output/";
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

void logJob(const char *vid, const int responseCode)
{
    struct tm const *_time = NULL;
    _time = createTime(_time);
    char *logOutput;
    char *test;
    test = __toString(0);

    logOutput = (char*)malloc(64);
    memset(logOutput, NULL, 64);
    char path[64] = "/Users/naver/Desktop/log/";

    sprintf(logOutput, "%sMERGE_%04d%02d%02d.log", path, _time->tm_year+1900, _time->tm_mon+1, _time->tm_mday);
    printf("%s\n", logOutput);
    FILE *logFP;

    logFP = fopen(logOutput, "a");
    if(!logFP)
    {
        fprintf(stderr, "fopen %d\n", responseCode);
        exit(OPENERR);
    }

    switch(responseCode)
    {
        case SUCCESS:
            fprintf(stderr, "Success, %d\n", responseCode);
            fprintf(logFP, "[%s] %04d%02d%02d%d_%s ,%d\n", test,
                    _time->tm_year+1900, _time->tm_mon+1, _time->tm_mday, _time->tm_sec,
                    vid, responseCode);
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

    fclose(logFP);
    free(logOutput);
}

int setInputSource(const char *path, char inputSource[][96])
{
    int i = 0;

    DIR *dp = NULL;
    struct dirent *entry = NULL;
    struct stat buf;
    char fullName[96];

    regex_t state;
    const char *extension = ".mp4";

    regcomp(&state, extension, REG_EXTENDED);

    if( (dp = opendir(path)) == NULL )
    {
        fprintf(stderr, "%s can not open\n", path);
        return -1;
    }

    while( (entry = readdir(dp)) != NULL )
    {
        if ( (regexec(&state, entry->d_name, 0, NULL, 0)) == 0 ) {
            printf("d_name : %s\n", entry->d_name);
            sprintf(fullName, "%s%s", path, entry->d_name);
            lstat(fullName, &buf);
            if (S_ISREG(buf.st_mode)) {
                sprintf(inputSource[i++], "%s", fullName);
            }
        }
    }

    closedir(dp);

    return 0;
}