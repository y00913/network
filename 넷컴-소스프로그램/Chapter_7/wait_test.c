//--------------------------------------------------------------
// 파일명 : wait_test.c
// 동  작 : wait() 함수 동작의 이해
// 컴파일 : gcc -o wait_test wait_test.c
// 실  행 : wait_test
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// 사용자 시그널 핸들러 함수
void catch_sigchld(int signo) { puts("###( Parent ) catch SIGCHLD "); }

int chstat;                  // 종료상태 값
int main(int argc, char *argv[]) {
    int i,n;
    struct sigaction sact;
    sact.sa_flags = 0;
    sigemptyset(&sact.sa_mask);
    sigaddset(&sact.sa_mask, SIGCHLD);

    // 시그널 핸들러 등록
    sact.sa_handler = SIG_DFL;       // 기본동작(무시)
    //sact.sa_handler = SIG_IGN;      // 시그널 무시 등록
    //sact.sa_handler = catch_sigchld;// 사용자 핸들러 등록
    sigaction(SIGCHLD, &sact, NULL);

    // 자식 프로세스 생성
    for(i=0; i<5; i++) {
        if(fork()==0) {
            if(i>2) sleep(6); // 부모의 wait() 호출보다 늦게 종료
            printf("(%d번 Child),PID=%d,PPID=%d Exited\n",i,getpid(),getppid());
            exit(13); // 종료값 13번
        }
    }
    // 부모 프로세스
    sleep(3); // 0,1,2 번 자식이 종료하기를 기다림
    puts("------------------");
    system("ps -a"); // 현재 시스템의 프로세스 상태확인
    puts("------------------");

    puts("#( Parent ) wait 호출함");
    for( ; ; ) {
        chstat = -1; // 초기화
        n = wait(&chstat);
        printf("# wait = %d(child stat=%d)\n",n, chstat);
        if( n == -1) {
            if(errno ==ECHILD) {
                perror("기다릴 자식프로세스가 존재하지 않음"); break;
            }
            else if(errno == EINTR) {
                perror("wait 시스템 콜이 인터럽트 됨"); continue;
            }
        }
    }
    puts("#( Parent ) 종료함");
    return 0;
}