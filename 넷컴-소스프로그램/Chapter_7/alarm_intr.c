//--------------------------------------------------------------
// 파일명 : alarm_intr.c
// 기  능 : alarm에 의한 read()의 인터럽트 확인
// 컴파일 : gcc -o alarm_intr alarm_intr.c
// 사용법 : alarm_intr
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

void catch(int sig) {
    printf("time out \n");
    return ;
}

int main(int argc, char *argv[]) {
    int  n;
    char buf[128];
    struct sigaction a;

    a.sa_handler = catch;
    a.sa_flags = 0;
    sigfillset(&a.sa_mask);
    sigaction(SIGALRM, &a, NULL);

    alarm(5);//5초 후에 SIGALARM 시그널 발생
    puts("input or 5 second wait");
    n = read(0, buf, sizeof(buf));
    if( n>=0 )
        printf("%d byte read success\n",n) ;
    else if(errno == EINTR)
        printf("read() interrupted\n");
    else
        perror("read fail");
    alarm(0);//타이머 구동을 종료
    return 0;
}