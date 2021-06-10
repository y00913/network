//--------------------------------------------------------------
// ���ϸ� : thread_basic.c
// ��  �� : �������� ������ ������ ID�� pid�� Ȯ��
// ������ : gcc -o thread_basic thread_basic.c -lpthread -D_REENTRANT
// ��  �� : thread_basic
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *thrfunc(void *arg) ;  // ������ ���� �Լ�

char who[10];
int main(int argc, char **argv) {
    int status;
    pthread_t tid;
    pid_t pid;

    // �ڽ� ���μ��� ����
    pid = fork();
    if(pid==0)
        sprintf(who,"child");
    else
        sprintf(who,"parent");

    // ���μ��� ID�� �ʱ� �������� ID Ȯ��
    printf("(%s's main) Process ID = %d\n", who, getpid());
    printf("(%s's main) Init thread ID = %d\n", who, pthread_self());

    // ���� �߻��� �����ڵ带 ����
    if( (status=pthread_create(&tid, NULL, &thrfunc, NULL))!=0) {
        printf("thread create error: %s\n", strerror(status));
        exit(0);
    }

    // ���ڷ� ������ ������  id�� �����ϱ⸦ ��ٸ�
    pthread_join(tid, NULL);
    printf("\n(%s)[%d] �����尡 �����߽��ϴ�\n", who, tid);
    return 0;
}

void *thrfunc(void *arg) {
    printf("(%s' thread routine) Process ID = %d\n", who, getpid());
    printf("(%s' thread routine) Thread ID = %d\n", who, pthread_self());
}