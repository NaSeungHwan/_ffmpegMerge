//
// Created by NAVER on 2017. 8. 28..
//

/**
 * Muxers는 인코딩 된 데이터를 AVPackets 형식으로 가져 와서 지정된 컨테이너 형식의 파일 또는 기타 출력 바이트 스트림에 씁니다.
 *
 * 다중화를 위한 주요 API 함수 는 파일 헤더를 작성하는 avformat_write_header () , 패킷을 작성하기 위해 av_write_frame () / av_interleaved_write_frame () 및 파일을 마무리하는 데 av_write_trailer () 를 사용합니다.
 * 다중화 프로세스가 시작되면 호출자는 먼저 avformat_alloc_context () 를 호출하여 다중화 컨텍스트를 만들어야합니다. 그런 다음 호출자는이 컨텍스트에서 다양한 필드를 채워서 muxer를 설정합니다.
 * oformat의 필드가 사용되는 먹서을 선택하도록 설정해야합니다.
 *
 * 형식이 AVFMT_NOFILE 유형이 아닌 경우, pb 필드는 avio_open2 () 에서 반환 된 열린 IO 컨텍스트 또는 사용자 정의 컨텍스트로 설정되어야합니다 .
 * 형식이 AVFMT_NOSTREAMS 유형이 아닌 경우 avformat_new_stream () 함수로 하나 이상의 스트림을 만들어야합니다 .
 * 발신자는 코덱 유형 , ID 및 기타 매개 변수 (예 : 너비 / 높이, 픽셀 또는 샘플 형식 등) 와 같은 스트림 코덱 매개 변수 정보를 입력해야 합니다.
 * 스트림의 타임베이스는 발신자가이 스트림에 대해 사용하고자하는 시간축으로 설정한다 (후술하는 바와 같이 실제로 먹서 의해 사용되는 시간 기준은 상이 할 수 있음에 유의).
 * 다시 매핑 하는 동안 avcodec_parameters_copy ()를 사용하는 대신 AVCodecParameters 의 관련 필드 만 수동으로 초기화하는 것이 좋습니다 .
 * 코덱 컨텍스트 값이 입력 및 출력 형식 컨텍스트 모두에서 유효하다는 보장이 없습니다.
 * 호출자는 AVFormatContext 문서에 설명 된 대로 글로벌 또는 스트림 별 메타 데이터, 챕터 , 프로그램 등과 같은 추가 정보를 입력 할 수 있습니다.
 * 그러한 정보가 출력물에 실제로 저장 될지 여부는 컨테이너 형식과 muxer가 지원하는 내용에 달려 있습니다.
 * 다중화 컨텍스트가 완전히 설정되면 호출자는 muxer 내부를 초기화하고 파일 헤더를 작성하기 위해 avformat_write_header () 를 호출해야합니다 .
 * 이 단계에서 IO 컨텍스트에 실제로 쓰여지는 것이 muxer에 달려 있지만,이 함수는 항상 호출되어야합니다. muxer 전용 옵션은 options 매개 변수에서이 함수로 전달되어야합니다.
 *
 * 그런 다음 av_write_frame () 또는 av_interleaved_write_frame () 을 반복적으로 호출하여 데이터를 muxer에 보내고
 * ( 해당 함수의 설명서에서 차이점에 대한 논의를 참조하십시오.이 중 하나만 단일 muxing 컨텍스트에서 사용할 수 있으며 혼합해서는 안됩니다 ).
 * muxer로 전송 된 패킷에 대한 타이밍 정보는 해당 AVStream 의 타임베이스 에 있어야합니다 .
 * 해당 타임베이스는 ( avformat_write_header () 단계에서) muxer에 의해 설정되며 호출자가 요청한 타임베이스와 다를 수 있습니다.
 * 모든 데이터가 기록되면, 호출자가 호출해야합니다 () av_write_trailer 버퍼 된 패킷을 플러시하고 출력 파일을 확정, 다음과 멀티플렉싱의 컨텍스트 IO 문맥을 닫습니다
 * (있는 경우) 그리고 마지막으로 free 해주기 위해 avformat_free_context()를 호출합니다 .
 */


#ifndef MERGE_IN_C_FFMPFUNCTHION_H
#define MERGE_IN_C_FFMPFUNCTHION_H

#include "lib/comm.h"


static void _initStream(stream *);

/**
 * soure를 open합니다
 *
 * @param target Stream 구조체, _initStream() 함수로 초기화가 필요합니다.
 *        lib/comm.h 에 선언되어있습니다
 * @param source const char * 타입의 타겟이 될 소스의 파일명입니다.
 *
 * @return 성공시 stream 갯수를 리턴하고, 실패시 -1 을 리턴합니다.
 **/
static int openInputSource(stream *, const char *);

static int initOutputSource(stream *, const char *, const char *);
static int mergeSource(stream *, const char [][96]);
static void _destroyStream(stream *);
int mergeMain(const char *, const char *);

#endif //MERGE_IN_C_FFMPFUNCTHION_H
