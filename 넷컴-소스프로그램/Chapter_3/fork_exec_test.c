//--------------------------------------------------------------
// 파일명 : fork_exec_test.c
// 동  작 : fork와 exec 함수를 이해하기 위한 예제
// 컴파일 : gcc -o fork_exec_test fork_exec_test.c
// 실  행 : fork_exec_test
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void child_start();

int main(int argc, char **argv) {
    pid_t pid;
    int child_status, child_return;

    puts("\t parent process start..............");
    if( (pid=fork()) < 0) {           // 에러 처리
        perror("fork failed : ");
        exit(0);
    }

    else if (pid == 0)              // 자식 프로세스
        child_start();
    else if( pid > 0)               // 부모 프로세스
        printf("\n\t** parent : [my pid: %d] my child pid = %d\n",getpid(), pid);
    return 0;
}

void child_start() {
    puts("\t child process start...");
    printf("\t** child : [my pid: %d] my parent pid = %d\n",getpid(), getppid());
    printf("\n\t** exec() 함수로 ls 명령을 수행합니다\n");
    execlp("ls", "ls", NULL);

    // 아래가 실행된다면 exec 함수에서 에러가 난 경우임
    perror("exec error at child: ");
    exit(0);
}