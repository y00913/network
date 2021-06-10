//--------------------------------------------------------------
// ���ϸ� : pen_and_note.c
// ��  �� : ������������ ������ ������ ��������� ����
// ������ : gcc -o pen_and_note  pen_and_note.c
// ��  �� : pen_and_note
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

// �������� ����
struct sembuf increase[] = {
    {0, +1, SEM_UNDO}, {1, +1, SEM_UNDO} };
struct sembuf decrease[] = {
    {0, -1, SEM_UNDO}, {1, -1, SEM_UNDO} };

// �������� �ʱⰪ, ���� �� �ڷ�� ��Ʈ �α�
unsigned short seminitval[] = { 1, 2 };

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
} semarg;

int semid;              // �������� ID
void do_work();         // �� ���μ����� ������ �۾�

int main(int argc, char *argv[]) {
    semid = semget(0x1234, 2, IPC_CREAT | 0600);
    if(semid ==-1)
        semid=semget(0x1234, 0, 0);//Ű�� 0x1234�� ���� ���������� ID�� ��ȯ

    // �������� �� �ʱ�ȭ
    semarg.array = seminitval;
    if(semctl(semid, 0, SETALL, semarg) == -1)
        errquit("semctl");

    // ǥ�� ��� non-buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // �� 4���� ���μ����� ����
    fork(); fork();
    do_work();
    semctl(semid,0,IPC_RMID,0);  // ���������� ����
    return 0;
}

void do_work() {
    int count=0;
#define Semop(val) if((semop val)==-1) errquit("semop")
    while(count<3) {
        Semop((semid, &decrease[PEN], 1));
        printf("[pid:%5d]������ ���\n", getpid());
        Semop((semid, &decrease[NOTE], 1));
        printf("\t[pid:%5d] ��Ʈ�� ���\n", getpid());

        printf("\t[pid:%5d] ���θ� ��\n", getpid());

        Semop((semid, &increase[PEN], 1));
        printf("\t[pid:%5d] ������ ��������\n", getpid());
        Semop((semid, &increase[NOTE], 1));
        printf("\t[pid:%5d] ��Ʈ�� ��������\n", getpid());

        sleep(1);
        count++;
    }
}