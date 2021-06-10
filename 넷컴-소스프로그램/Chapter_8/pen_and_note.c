//--------------------------------------------------------------
// 파일명 : pen_and_note.c
// 동  작 : 공유데이터의 경쟁적 접근을 세마포어로 제어
// 컴파일 : gcc -o pen_and_note  pen_and_note.c
// 실  행 : pen_and_note
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void errquit(char *mesg) {
    perror(mesg);
    exit(0);
}

#define PEN 0
#define NOTE 1

// 세마포어 조작
struct sembuf increase[] = {
    {0, +1, SEM_UNDO}, {1, +1, SEM_UNDO} };
struct sembuf decrease[] = {
    {0, -1, SEM_UNDO}, {1, -1, SEM_UNDO} };

// 세마포어 초기값, 연필 한 자루와 노트 두권
unsigned short seminitval[] = { 1, 2 };

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
} semarg;

int semid;              // 세마포어 ID
void do_work();         // 각 프로세스가 수행할 작업

int main(int argc, char *argv[]) {
    semid = semget(0x1234, 2, IPC_CREAT | 0600);
    if(semid ==-1)
        semid=semget(0x1234, 0, 0);//키를 0x1234를 갖는 세마포어의 ID를 반환

    // 세마포어 값 초기화
    semarg.array = seminitval;
    if(semctl(semid, 0, SETALL, semarg) == -1)
        errquit("semctl");

    // 표준 출력 non-buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // 총 4개의 프로세스를 만듬
    fork(); fork();
    do_work();
    semctl(semid,0,IPC_RMID,0);  // 세마포어의 삭제
    return 0;
}

void do_work() {
    int count=0;
#define Semop(val) if((semop val)==-1) errquit("semop")
    while(count<3) {
        Semop((semid, &decrease[PEN], 1));
        printf("[pid:%5d]연필을 들고\n", getpid());
        Semop((semid, &decrease[NOTE], 1));
        printf("\t[pid:%5d] 노트를 들고\n", getpid());

        printf("\t[pid:%5d] 공부를 함\n", getpid());

        Semop((semid, &increase[PEN], 1));
        printf("\t[pid:%5d] 연필을 내려놓음\n", getpid());
        Semop((semid, &increase[NOTE], 1));
        printf("\t[pid:%5d] 노트를 내려놓음\n", getpid());

        sleep(1);
        count++;
    }
}