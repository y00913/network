//--------------------------------------------------------------
// 파일명 : catch_sigint4.c
// 동  작 : SIGINT 시그널에 대한 핸들러 등록하는 예
// 컴파일 : gcc -o catch_sigint1  catch_sigint1.c
// 실  행 : catch_sigint4
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int count;

// 시그널 핸들러
void catch_sigint(int  signum) {
    static int call_count = 0; // 정적 변수 사용
    printf("catch_sigint %d번째 호출 \n", call_count++);
    sleep(3);
    printf("catch_sigint 리턴\n");
    return;
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    sigset_t masksets;
    int n;
    char buf[10];
    sigfillset(&masksets);

    // 시그널 핸들러 설치
    act.sa_handler = catch_sigint;

    // 시그널 핸들러가 실행되는 동안 모든 시그널을 블록함
    act.sa_mask = masksets;
    act.sa_flags = SA_RESTART | SA_NODEFER;
    sigaction(SIGINT, &act, NULL);// 시스널 핸들러 등록

    for(count=0; count<3 ; count++ ) {
       n = read(0, buf, sizeof(buf));
       if(n==-1 && errno ==EINTR)
	   printf("read 함수 Interrrupted\n");
       else
	   printf("\t** %d byte input\n", n);
    }
    return 0;
}