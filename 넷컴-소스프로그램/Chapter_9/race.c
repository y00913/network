//--------------------------------------------------------------
// 파일명 :  race.c
// 기  능 :  스레드 동기화문제(경쟁 조건)의 발생
// 컴파일 :  gcc -o race race.c -lpthread -D_REENTRANT
// 실  행 :  race
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THR  2
void *thrfunc(void *arg);      // 스레드 시작 함수
void prn_data(long who);       // 스레드 ID 출력
int who_run = -1;              // prn_data() 수행중인 스레드 ID, 초기값은 -1

int main(int argc, char **argv) {
    pthread_t tid[MAX_THR];
    int i, status;
    for(i=0; i<MAX_THR; i++) {
        if( (status=pthread_create(&tid[i], NULL, &thrfunc, NULL))!=0) {
            printf("thread create error: %s\n",strerror(status));
            exit(0);
        }
    }
    pthread_join(tid[0],NULL);
    return 0;
}

void *thrfunc(void *arg) {
    while(1) {
        prn_data(pthread_self());
    }
    return NULL;
}

void prn_data(long me) {
    who_run = me;
    if(who_run != pthread_self()) {
        printf("Error : %d스레드 실행중 who_run=%d\n", me, who_run);
    }
    who_run = -1;     // 초기값으로 환원
}