//--------------------------------------------------------------
// ���ϸ� : cond.c
// ��  �� : ���Ǻ����� ���ؼ� �� �����尣�� ����ϴ� ��
// ������ : gcc -o cond cond.c -lpthread -D_REENTRANT
// ��  �� : cond  5 (�ڽ� �����尡 sleep�� �ð��� ����)
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

// ���ؽ�, ���Ǻ��� �� ����������
typedef struct _complex {
    pthread_mutex_t mutex;     // ���ؽ�
    pthread_cond_t cond ;      // ���Ǻ���
    int value;                 // ����������
} thread_control_t;

// ���ؽ�, ���Ǻ���, ������������ �ʱ�ȭ
thread_control_t data = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};

// ���� ��� �� ������ ����
void thr_errquit(char *msg,int errcode) {
    printf("%s: %s\n",msg,strerror(errcode));
    pthread_exit(NULL);
}

void *wait_thread(void *arg);     // ������ ���� �Լ�
int sleep_time ;                 // �ڽ� �����尡 ó�� ���ڴ� �ð�

int main(int argc, char **argv) {
    int status;
    pthread_t wait_thr;
    struct timespec timeout;
    if (argc != 2 ) {
        printf("���� :  cond  5 (�ڽ� �����尡 sleep�� �ð�)\n");
        exit(0);
    }
    sleep_time = atoi(argv[1]);
    if( (status=pthread_create(&wait_thr, NULL, wait_thread, NULL))!=0) {
        printf("pthread_create fail : %s\n",strerror(status));
        exit(1);
    }

    timeout.tv_sec = time(NULL)+3;     // ������ �ð� + 3��
    timeout.tv_nsec = 0; // nano second

    // ���ؽ� ���
    if( (status=pthread_mutex_lock(&data.mutex)) != 0) {
        printf("pthread_mutex_lock fail : %s\n",strerror(status));
        exit(1);
    }

    if (data.value == 0) {
        status = pthread_cond_timedwait(&data.cond, &data.mutex, &timeout);//���ؽ� ����
        if(status == ETIMEDOUT) {
            printf("Condtion wait time out\n");
        }
        else {
            printf("Wait on Condition.....\n");
        }
    }

    // data.value ���� �ڽ� �����忡�� 1�� �ٲ�
    if(data.value == 1)
        printf("Condition was signaled.\n");
    if( (status=pthread_mutex_unlock(&data.mutex)) != 0) {
        printf("pthread mutex_unlock fail : %s\n",strerror(status));
        exit(0);
    }

    // �ڽ� �����尡 �����ϱ⸦ ��ٸ�
    if( (status=pthread_join(wait_thr, NULL)) != 0) {
        printf("pthread_join: %s\n",status);
        exit(0);
    }
    return 0;
}

// ������ ���� �Լ�
void *wait_thread(void *arg) {
    int status;
    sleep(sleep_time);
    if((status=pthread_mutex_lock(&data.mutex)) != 0)
       thr_errquit("pthread_mutex_lock failure",status);
    data.value = 1;
    if((status=pthread_cond_signal(&data.cond)) != 0)
       thr_errquit("pthread_cond_signalure",status);
    if((status=pthread_mutex_unlock(&data.mutex)) != 0)
       thr_errquit("pthread_mutex_unlock failure",status);
    return NULL;
}