//--------------------------------------------------------------
// ���ϸ� :  race.c
// ��  �� :  ������ ����ȭ����(���� ����)�� �߻�
// ������ :  gcc -o race race.c -lpthread -D_REENTRANT
// ��  �� :  race
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THR  2
void *thrfunc(void *arg);      // ������ ���� �Լ�
void prn_data(long who);       // ������ ID ���
int who_run = -1;              // prn_data() �������� ������ ID, �ʱⰪ�� -1

int main(int argc, char **argv) {
    pthread_t tid[MAX_THR];
    int i, status;
    for(i=0; i<MAX_THR; i++) {
        if( (status=pthread_create(&tid[i], NULL, &thrfunc, NULL))!=0) {
            printf("thread create error: %s\n",strerror(status));
            exit(0);
        }
    }
    pthread_join(tid[0],NULL);
    return 0;
}

void *thrfunc(void *arg) {
    while(1) {
        prn_data(pthread_self());
    }
    return NULL;
}

void prn_data(long me) {
    who_run = me;
    if(who_run != pthread_self()) {
        printf("Error : %d������ ������ who_run=%d\n", me, who_run);
    }
    who_run = -1;     // �ʱⰪ���� ȯ��
}