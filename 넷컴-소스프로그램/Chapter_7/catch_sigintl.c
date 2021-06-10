//--------------------------------------------------------------
// 파일명 : catch_sigint1.c
// 동  작 : SIGINT 시그널에 대한 핸들러 등록하는 예
// 컴파일 : gcc -o catch_sigint1  catch_sigint1.c
// 실  행 : catch_sigint1
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
    printf("\n(count=%d)  CTRL-C pressed!\n", count);
    return;
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    sigset_t masksets;
    int i;
    char buf[10];
    sigfillset(&masksets);

    // 시그널 핸들러 설치
    act.sa_handler = catch_sigint;

    // 시그널 핸들러가 실행되는 동안 모든 시그널을 블록함
    act.sa_mask = masksets;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);// 시스널 핸들러 등록

    for(count=0; count<3 ; count++ )
       read(0, buf, sizeof(buf));
    return 0;
}