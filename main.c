#include <stdio.h>
#include "mergeFunction.h"


int main() {

    int ret = 0;
    //logJob("test_010101", SUCCESS);

    ret = mergeMain("real_010102", "/Users/naver/Desktop/");
    return ret;
}

/*
 *
 *
void task1(){
    printf("Thread #%u working on task1\n", (int)pthread_self());
}


void task2(){
    printf("Thread #%u working on task2\n", (int)pthread_self());
}
 char path[] = "/bin/ls";
    char *arg[] = {"ls", "-al", NULL};

    puts("Making threadpool with 4 threads");
    threadpool thpool = thpool_init(10);

    puts("Adding 40 tasks to threadpool");
    int i;
    for (i=0; i<20; i++){
        thpool_add_work(thpool, (void*)task1, NULL);
        thpool_add_work(thpool, (void*)task2, NULL);
    };

    puts("Killing threadpool");
    thpool_destroy(thpool);

    printf("Hello, World!\n");
    printf("%d\n", _processPool(path, arg));
char path[256];
realpath("20170801_real_37493_JGAs7dKftM_M.mp4",path);
printf("%s\n", path);
memset(path, 0x00, sizeof(path));
readlink("/User", path, 256);

fprintf(stderr,"arBuf[%s]\n", path);

return 0;

* */