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


void logJob(const char *vid, const char *disc)
{
    struct tm const *_time = NULL;
    _time = createTime(_time);
    char *logOutput;

    logOutput = (char*)malloc(64);
    memset(logOutput, NULL, 64);
    char path[64] = "/Users/naver/Desktop/log/";

    sprintf(logOutput, "%s%s_%04d%02d%02d.log", path, vid, _time->tm_year+1900, _time->tm_mon+1, _time->tm_mday);
    FILE *logFP;

    logFP = fopen(logOutput, "a");
    if(!logFP)
    {
        perror("logfile open error\n");
        exit(-1);
    }
    fprintf(logFP, "[%s] %04d%02d%02d%d_%s %s\n", "OK",
            _time->tm_year+1900, _time->tm_mon+1, _time->tm_mday, _time->tm_sec,
            vid, disc);


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
        fprintf(stderr, "can not open dir : %s\n", path);
        return -1;
    }

    while( (entry = readdir(dp)) != NULL )
    {
        if ( (regexec(&state, entry->d_name, 0, NULL, 0)) == 0 ) {
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