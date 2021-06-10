//------------------------------------------------------------
// ���ϸ� : shmcontrol.c
// ��  �� : ������� �̿��� �����޸� ����ȭ���� �ذ�
// ������ : gcc -o shmcontrol shmcontrol.c
// ��  �� : shmcontrol  shmkey  semkey
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

// �������� ���� �����ϴ� sembuf
struct sembuf waitsem[] = { {0, -1, 0} };
struct sembuf notifysem[] = { {0, +1, 0} };

// semop �Լ� wrapper ��ũ��
#define Semop(val)      { if(semop val==-1) errquit("semop fail");   }

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
} semarg ;

void errquit(char *msg){perror(msg); exit(1);}

// �ڽ� ���μ����� �����Ͽ� busy() �Լ��� ȣ��
void fork_and_run();

void busy();          // �� ���μ����� �����޸𸮿� �����ϴ� �Լ�
void access_shm();    // �����޸𸮿� �����ϴ� �Լ�
char *shm_data;       // �����޸𸮿� ���� ������
int shmid, semid;     // �����޸� ID, �������� ID

int main(int argc, char *argv[]) {
    key_t shmkey, semkey;          // �����޸�, �������� Ű
    unsigned short initsemval[1];  // ������ �ʱⰪ
    if(argc<2) {
        printf("Usage : %s  shmkey  semkey\n",argv[0]);
        exit(1);
    }
    shmkey = atoi(argv[1]); // �����޸� Ű
    semkey = atoi(argv[2]); // �������� Ű

    // �����޸� ����
    shmid = shmget(shmkey, 128, IPC_CREAT|0660);
    if(shmid<0)
        errquit("shmget fail ");
    // �����޸� ÷��
    shm_data = (char *)shmat(shmid, (void*)0, 0);
    if(shm_data == (char*)-1)
        errquit("shmat fail");
    // �������� ����
    semid = semget(semkey, 1, IPC_CREAT|0660);
    if(semid == -1)
        errquit("semget fail ");

    // ���������� �ʱⰪ�� 1�� ����
    // �����޸𸮿� �� ���μ������� ���ٰ�����
    initsemval[0] = 1;
    semarg.array = initsemval;
      if( semctl(semid, 0, SETALL, semarg)==-1)
        errquit("semctl ");

    fork_and_run();   // �ڽ� ���μ��� ����
    fork_and_run();   // �ڽ� ���μ��� ����
    busy();           // �θ� ���μ����� �����޸� ����
    wait(NULL);       // �ڽ� ���μ����� �����⸦ ��ٸ�
    wait(NULL);       // �ڽ� ���μ����� �����⸦ ��ٸ�
    shmctl(shmid, IPC_RMID, 0);      // �����޸� ����
    semctl(semid, 0, IPC_RMID, 0);   // �������� ����
    return 0;
}

// �ڽ� ���μ��� ���� ��  busy() ����
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

// �ڽ� ���μ����� �����ϴ� �Լ�
void busy() {
    int i=0;
    for( i=0; i<100; i++ )  {   // 100�� ����
        Semop((semid, &waitsem[0], 1));//�������� ����
        access_shm();
        Semop((semid, &notifysem[0], 1));//�������� ����
    }
    shmdt(shm_data);           // �����޸� �и�
}

// �����޸𸮿� ������ �ϴ� �κ�
void access_shm() {
    int i;
	struct shmid_ds *buf;
    pid_t pid;
    struct timespec  ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000;    // 0.1 ��
    // �����޸𸮿� �ڽ��� pid�� ���
    sprintf(shm_data, "%d", getpid());
    // �����޸� ���� �ð�
    for(i=0; i<1000; i++) ;
    pid = atoi(shm_data);
    // �����޸𸮿� ����� pid�� �ڽ��� pid�� �ƴϸ� Error
    if(pid != getpid())
        puts("Error : �ٸ� ���μ����� ���ÿ� �����޸� ������\n");
    else {
        putchar('.');            // ok
    }
    fflush(stdout);
    //nanosleep(&ts, NULL);      // sleep
    sleep(0.5);      // sleep
    return ;
}
