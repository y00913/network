//------------------------------------------------------------
// 파일명 : shmcontrol.c
// 동  작 : 세마포어를 이용한 공유메모리 동기화문제 해결
// 컴파일 : gcc -o shmcontrol shmcontrol.c
// 실  행 : shmcontrol  shmkey  semkey
//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

// 세마포어 값을 조절하는 sembuf
struct sembuf waitsem[] = { {0, -1, 0} };
struct sembuf notifysem[] = { {0, +1, 0} };

// semop 함수 wrapper 매크로
#define Semop(val)      { if(semop val==-1) errquit("semop fail");   }

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
} semarg ;

void errquit(char *msg){perror(msg); exit(1);}

// 자식 프로세스를 생성하여 busy() 함수를 호출
void fork_and_run();

void busy();          // 각 프로세스가 공유메모리에 접근하는 함수
void access_shm();    // 공유메모리에 접근하는 함수
char *shm_data;       // 공유메모리에 대한 포인터
int shmid, semid;     // 공유메모리 ID, 세마포어 ID

int main(int argc, char *argv[]) {
    key_t shmkey, semkey;          // 공유메모리, 세마포어 키
    unsigned short initsemval[1];  // 세마포 초기값
    if(argc<2) {
        printf("Usage : %s  shmkey  semkey\n",argv[0]);
        exit(1);
    }
    shmkey = atoi(argv[1]); // 공유메모리 키
    semkey = atoi(argv[2]); // 세마포어 키

    // 공유메모리 생성
    shmid = shmget(shmkey, 128, IPC_CREAT|0660);
    if(shmid<0)
        errquit("shmget fail ");
    // 공유메모리 첨부
    shm_data = (char *)shmat(shmid, (void*)0, 0);
    if(shm_data == (char*)-1)
        errquit("shmat fail");
    // 세마포어 생성
    semid = semget(semkey, 1, IPC_CREAT|0660);
    if(semid == -1)
        errquit("semget fail ");

    // 세마포어의 초기값을 1로 설정
    // 공유메모리에 한 프로세스만이 접근가능함
    initsemval[0] = 1;
    semarg.array = initsemval;
      if( semctl(semid, 0, SETALL, semarg)==-1)
        errquit("semctl ");

    fork_and_run();   // 자식 프로세스 생성
    fork_and_run();   // 자식 프로세스 생성
    busy();           // 부모 프로세스도 공유메모리 접근
    wait(NULL);       // 자식 프로세스가 끝나기를 기다림
    wait(NULL);       // 자식 프로세스가 끝나기를 기다림
    shmctl(shmid, IPC_RMID, 0);      // 공유메모리 삭제
    semctl(semid, 0, IPC_RMID, 0);   // 세마포어 삭제
    return 0;
}

// 자식 프로세스 생성 및  busy() 수행
void fork_and_run() {
    pid_t pid = fork();
    if (pid < 0)
        errquit("fork fail");
    else if(pid == 0) {
        busy(); // child  race
        exit(0);
    }
    return;
}

// 자식 프로세스가 수행하는 함수
void busy() {
    int i=0;
    for( i=0; i<100; i++ )  {   // 100번 접근
        Semop((semid, &waitsem[0], 1));//세마포어 감소
        access_shm();
        Semop((semid, &notifysem[0], 1));//세마포어 증가
    }
    shmdt(shm_data);           // 공유메모리 분리
}

// 공유메모리에 접근을 하는 부분
void access_shm() {
    int i;
	struct shmid_ds *buf;
    pid_t pid;
    struct timespec  ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000;    // 0.1 초
    // 공유메모리에 자신의 pid를 기록
    sprintf(shm_data, "%d", getpid());
    // 공유메모리 접근 시간
    for(i=0; i<1000; i++) ;
    pid = atoi(shm_data);
    // 공유메모리에 기록한 pid가 자신의 pid가 아니면 Error
    if(pid != getpid())
        puts("Error : 다른 프로세스도 동시에 공유메모리 접근함\n");
    else {
        putchar('.');            // ok
    }
    fflush(stdout);
    //nanosleep(&ts, NULL);      // sleep
    sleep(0.5);      // sleep
    return ;
}
