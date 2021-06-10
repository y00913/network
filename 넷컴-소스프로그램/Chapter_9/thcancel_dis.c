//--------------------------------------------------------------
// ���ϸ� : thcancel_dis.c
// ��  �� : �������� ��ҿ�û�� �����ϴ� ��
// ������ : gcc -o thcancel_dis thcancel_dis.c -lpthread -D_REENTRANT
// ��  �� : thcancel_dis
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>

// thread start routine
void *thrfunc(void *arg) ;

// ��ҿ�û�� ������ �������� ���Ḧ ��ٸ��� �Լ�
void cancel_and_join(int tid);
int max_loop=20000000;
int cnt=0;
pthread_t curthd;

int main(int argc, char **argv) {
    pthread_t tid;
    int status, i;
    struct timespec micro_sec = {0,100000};

    // cancel_disable�� �׽�Ʈ
    printf("** PTHREAD_CANCEL_DISABLE\n");
    if((status=pthread_create(&tid, NULL, &thrfunc, NULL))!=0) {
        printf("thread create error: %s\n", strerror(status));
        exit(0);
    }
    // ������ ���� ���� ��ҿ�û�� �����ϵ��� sleep
    nanosleep(&micro_sec, NULL);
    // ��ҿ�û�� ����
    cancel_and_join(tid);
    return 0;
}

// ������ ���� �Լ�
void *thrfunc(void *arg) {
    int status;
    curthd = pthread_self();
    // ��ҿ�û�� �����ϵ��� ����
    if( (status=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL))!=0){
        printf("pthread_setcancelstate fail: %s\n", strerror(status));
        exit(0);
    }

    // ���ѷ����� ���鼭 50,000��° ���� ��ҿ�û�� �˻�
    for(cnt=1; cnt<max_loop; cnt++)
        if(cnt%50000 == 0)
            pthread_testcancel();
}

// �����忡�� ��ҿ�û�� ������ �����ϱ⸦ ��ٸ��� �Լ� ����
void cancel_and_join(int tid) {
    void *result;
    int status;
    if((status = pthread_cancel(tid)) != 0) {
        printf("pthread_cancel error : %s\n", strerror(status));
        exit(0);
    }
    // tid �����尡 �����ϱ⸦ ��ٸ�
    if((status = pthread_join(tid, &result)) != 0) {
        printf("pthread join error: %s\n", strerror(status));
        exit(0);
    }

    // ��ҿ�û�� ���� ����� ��� PTHREAD_CANCELED�� ���ϵ�
    if(result == PTHREAD_CANCELED)
        printf("[Thread ID=%d] thread is canceled \n", curthd);
    else
        printf("[Thread ID=%d] thread is not canceled\n", curthd);

    printf("�� %d ���� ������ %d ���� ������ ������\n\n", max_loop, cnt);
    return ;
}