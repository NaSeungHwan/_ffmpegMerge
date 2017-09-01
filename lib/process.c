//
// Created by NAVER on 2017. 8. 28..
//
#include <unistd.h>
#include <stdlib.h>
#include "libavformat/avformat.h"

int _processPool(const char *path, char *const argv[])
{
    int ret;
    pid_t pid;

    pid = fork();

    if( pid == -1 ){ perror("fork"); exit(EXIT_FAILURE); }
    if( pid == 0 ) {
        ret =
        ret = execv(path, argv);
        if (ret == -1) { perror("execv"); exit(EXIT_FAILURE); }
    }
    waitpid(-1, NULL, 0);
    return 0;
}