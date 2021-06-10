//--------------------------------------------------------------
// ���ϸ� : shmbusyaccess.c
// ��  �� : �����޸𸮿��� ����ȭ������ ����
// ������ : gcc -o shmbusyaccess shmbusyaccess.c
// ��  �� : shmbusyaccess   shmkey
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
// �ڽ� ���μ����� �����Ͽ� busy() �Լ��� ȣ���ϵ��� ��
void fork_and_run();
// �� ���μ����� �����޸𸮿� ���������� �����ϴ� �Լ�
void busy();
// �����޸𸮿� �����ϴ� �Լ�
void access_shm(int count);
char *shm_data;        // �����޸� ������
int shmid;             // �����޸� ID

int main(int argc, char *argv[]) {
    key_t  shmkey;     // �����޸� Ű

    if(argc<2) {
        printf("Usage : %s  shmkey\n",argv[0]);
        exit(1);
    }
    shmkey = atoi(argv[1]);
    shmid = shmget(shmkey, 128, IPC_CREAT | 0660);//�����޸� ����
    if(shmid < 0)
        errquit("shmget fail ");
    shm_data = (char *)shmat(shmid, (void*)0, 0);//�����޸� ÷��, �ڵ��ּҼ���, �б⾲��
    if(shm_data == (char*)-1)
        errquit("shmat fail");
    fork_and_run();//�ڽ� ���μ��� ����
    fork_and_run();
    busy();
    wait(NULL);//�ڽ� ���μ����� ���� �������� ���
    shmctl(shmid,IPC_RMID,0); // �����޸� ����
    return 0;
}

// �ڽ� ���μ��� ���� ��  busy() ����
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

// �ڽ� ���μ����� �����ϴ� �Լ�
void busy() {
    int i=0;
    for( i=0;i<500000 ;i++ )
        access_shm(i);
    shmdt(shm_data);       // �����޸� �и�
}

// �����޸𸮿� ������ �ϴ� �κ�
void access_shm(int count) {
    int i;
    pid_t pid;

    // �����޸𸮿� �ڽ��� PID ���
    sprintf(shm_data, "%d", getpid());
    // ���� : �����޸� ���� �ð��� ����
    for(i=0; i<1000; i++);
    pid = atoi(shm_data);
    // �����޸𸮿� ����� PID�� �ڽ��� PID�� �ƴϸ� Error
    if(pid != getpid())
        printf("Error(count=%d) : �ٸ� ���μ����� ���ÿ� �����޸� ������\n",count);
    else {
        // �����̸� �ƹ� ����� ���� ����
    }
    return;
}