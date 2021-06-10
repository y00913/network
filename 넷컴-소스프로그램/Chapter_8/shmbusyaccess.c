//--------------------------------------------------------------
// 파일명 : shmbusyaccess.c
// 동  작 : 공유메모리에서 동기화문제를 보임
// 컴파일 : gcc -o shmbusyaccess shmbusyaccess.c
// 실  행 : shmbusyaccess   shmkey
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

void errquit(char *msg){perror(msg); exit(1);}
// 자식 프로세스를 생성하여 busy() 함수를 호출하도록 함
void fork_and_run();
// 각 프로세스가 공유메모리에 경쟁적으로 접근하는 함수
void busy();
// 공유메모리에 접근하는 함수
void access_shm(int count);
char *shm_data;        // 공유메모리 포인터
int shmid;             // 공유메모리 ID

int main(int argc, char *argv[]) {
    key_t  shmkey;     // 공유메모리 키

    if(argc<2) {
        printf("Usage : %s  shmkey\n",argv[0]);
        exit(1);
    }
    shmkey = atoi(argv[1]);
    shmid = shmget(shmkey, 128, IPC_CREAT | 0660);//공유메모리 생성
    if(shmid < 0)
        errquit("shmget fail ");
    shm_data = (char *)shmat(shmid, (void*)0, 0);//공유메모리 첨부, 자동주소설정, 읽기쓰기
    if(shm_data == (char*)-1)
        errquit("shmat fail");
    fork_and_run();//자식 프로세스 생성
    fork_and_run();
    busy();
    wait(NULL);//자식 프로세스의 종료 시점까지 대기
    shmctl(shmid,IPC_RMID,0); // 공유메모리 제거
    return 0;
}

// 자식 프로세스 생성 및  busy() 수행
void fork_and_run() {
    pid_t pid = fork();
    if (pid < 0)
        errquit("fork fail");
    else if(pid == 0) {
        busy();
        exit(0);
    }
    return ;
}

// 자식 프로세스가 수행하는 함수
void busy() {
    int i=0;
    for( i=0;i<500000 ;i++ )
        access_shm(i);
    shmdt(shm_data);       // 공유메모리 분리
}

// 공유메모리에 접근을 하는 부분
void access_shm(int count) {
    int i;
    pid_t pid;

    // 공유메모리에 자신의 PID 기록
    sprintf(shm_data, "%d", getpid());
    // 지연 : 공유메모리 접근 시간에 포함
    for(i=0; i<1000; i++);
    pid = atoi(shm_data);
    // 공유메모리에 기록한 PID가 자신의 PID가 아니면 Error
    if(pid != getpid())
        printf("Error(count=%d) : 다른 프로세스도 동시에 공유메모리 접근함\n",count);
    else {
        // 정상이며 아무 출력을 하지 않음
    }
    return;
}