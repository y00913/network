//--------------------------------------------------------------
// 파일명 : thread_basic.c
// 동  작 : 스레드의 생성과 스레드 ID와 pid를 확인
// 컴파일 : gcc -o thread_basic thread_basic.c -lpthread -D_REENTRANT
// 실  행 : thread_basic
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *thrfunc(void *arg) ;  // 스레드 시작 함수

char who[10];
int main(int argc, char **argv) {
    int status;
    pthread_t tid;
    pid_t pid;

    // 자식 프로세스 생성
    pid = fork();
    if(pid==0)
        sprintf(who,"child");
    else
        sprintf(who,"parent");

    // 프로세스 ID와 초기 스레드의 ID 확인
    printf("(%s's main) Process ID = %d\n", who, getpid());
    printf("(%s's main) Init thread ID = %d\n", who, pthread_self());

    // 에러 발생시 에러코드를 리턴
    if( (status=pthread_create(&tid, NULL, &thrfunc, NULL))!=0) {
        printf("thread create error: %s\n", strerror(status));
        exit(0);
    }

    // 인자로 지정한 스레드  id가 종료하기를 기다림
    pthread_join(tid, NULL);
    printf("\n(%s)[%d] 스레드가 종료했습니다\n", who, tid);
    return 0;
}

void *thrfunc(void *arg) {
    printf("(%s' thread routine) Process ID = %d\n", who, getpid());
    printf("(%s' thread routine) Thread ID = %d\n", who, pthread_self());
}