cmake_minimum_required(VERSION 3.8)
project(merge_in_C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c lib/thpool.c lib/thpool.h mergeFunction.c mergeFunction.h lib/comm.h lib/comm.c)
add_executable(merge_in_C ${SOURCE_FILES})

include_directories(/usr/local/Cellar/ffmpeg/3.3.3/include)
link_directories(ffmpeg_test /usr/local/Cellar/ffmpeg/3.3.3/lib)

target_link_libraries(
        merge_in_C
        avcodec
        avdevice
        avfilter
        avformat
        avresample
        avutil
        postproc
        swresample
        swscale
)