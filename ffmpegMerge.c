//
// Created by NAVER on 2017. 8. 29..
//
#include <stdio.h>
#include "libavformat/avformat.h"
#include "mergeFunction.h"
#include "ffmpegMerge.h"


/** main function **/
int mergeMain(const char *vid) {

    /** variable definition **/
    int ret = SUCCESS; // process status
    // input source 추후, 특정 path내의 미디어 파일을 모두 찾아 넣도록 할 예정
    const char *source[] = {"/Users/naver/Desktop/20170801_real_37493_JGAs7dKftM_M.mp4"};
    stream target; // merge 에 필요한 변수들이 있는 struct

    /** init source **/
    _initStream(&target);
    ret = initOutputSource(&target, source[0], vid);


    fprintf(stderr, "mergeMain return : %d\n", ret );
    return ret;
}