//--------------------------------------------------------------
// 파일명 : thcancel_dis.c
// 동  작 : 스레드의 취소요청을 무시하는 예
// 컴파일 : gcc -o thcancel_dis thcancel_dis.c -lpthread -D_REENTRANT
// 실  행 : thcancel_dis
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>

// thread start routine
void *thrfunc(void *arg) ;

// 취소요청을 보내고 스레드의 종료를 기다리는 함수
void cancel_and_join(int tid);
int max_loop=20000000;
int cnt=0;
pthread_t curthd;

int main(int argc, char **argv) {
    pthread_t tid;
    int status, i;
    struct timespec micro_sec = {0,100000};

    // cancel_disable을 테스트
    printf("** PTHREAD_CANCEL_DISABLE\n");
    if((status=pthread_create(&tid, NULL, &thrfunc, NULL))!=0) {
        printf("thread create error: %s\n", strerror(status));
        exit(0);
    }
    // 루프가 도는 도중 취소요청이 도착하도록 sleep
    nanosleep(&micro_sec, NULL);
    // 취소요청을 보냄
    cancel_and_join(tid);
    return 0;
}

// 스레드 시작 함수
void *thrfunc(void *arg) {
    int status;
    curthd = pthread_self();
    // 취소요청을 무시하도록 설정
    if( (status=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL))!=0){
        printf("pthread_setcancelstate fail: %s\n", strerror(status));
        exit(0);
    }

    // 무한루프를 돌면서 50,000번째 마다 취소요청을 검사
    for(cnt=1; cnt<max_loop; cnt++)
        if(cnt%50000 == 0)
            pthread_testcancel();
}

// 스레드에게 취소요청을 보내고 종료하기를 기다리는 함수 내용
void cancel_and_join(int tid) {
    void *result;
    int status;
    if((status = pthread_cancel(tid)) != 0) {
        printf("pthread_cancel error : %s\n", strerror(status));
        exit(0);
    }
    // tid 스레드가 종료하기를 기다림
    if((status = pthread_join(tid, &result)) != 0) {
        printf("pthread join error: %s\n", strerror(status));
        exit(0);
    }

    // 취소요청에 의해 종료된 경우 PTHREAD_CANCELED이 리턴됨
    if(result == PTHREAD_CANCELED)
        printf("[Thread ID=%d] thread is canceled \n", curthd);
    else
        printf("[Thread ID=%d] thread is not canceled\n", curthd);

    printf("총 %d 번의 루프중 %d 번의 루프를 돌았음\n\n", max_loop, cnt);
    return ;
}