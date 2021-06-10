//--------------------------------------------------------------
// 파일명 :  mutextest.c
// 기  능 :  1초 간격으로 두개의 스레드가 돌아가면서 count 출력
// 컴파일 :  gcc -o mutextest mutextest.c -lpthread -D_REENTRANT
// 사용법 :  mutextest
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// 스레드 시작 함수
void *thrfunc(void *arg) ;
int counting = 0;                  // 공유데이터
pthread_mutex_t count_lock;        // 뮤텍스 변수
pthread_mutexattr_t mutex_attr;    // 뮤텍스 속성 변수

int main(int argc, char **argv) {
    pthread_t tid[2];
    int i, status;
    // 뮤텍스 속성 변수를초기화하고, 속성들을 기본값으로 설정한다.
    pthread_mutexattr_init(&mutex_attr); 
    pthread_mutex_init(&count_lock, &mutex_attr); // 뮤텍스 초기화
    for(i=0; i <2; i++) {
        // 에러 발생시 non-zero 값 리턴
        if((status=pthread_create(&tid[i], NULL, &thrfunc, NULL))!=0) {
            printf("pthread_create fail: %s",strerror(status));
            exit(0);
        }
    }
    for(i=0; i<2; i++) pthread_join(tid[i], NULL);
    return 0;
}

// 스레드 시작 함수
void *thrfunc(void *arg) {
    while(1) {
        pthread_mutex_lock( &count_lock );
        printf("\n[%ld 스레드] 뮤텍스 잠금\n", pthread_self());
        printf("[%ld 스레드] counting = %d\n", pthread_self(), counting);
        counting++;
        sleep(1);
        printf("[%ld 스레드] 뮤텍스 해제\n", pthread_self());
        pthread_mutex_unlock(&count_lock);
    }
    return NULL;
}