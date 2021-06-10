//--------------------------------------------------------------
// ���ϸ� :  mutextest.c
// ��  �� :  1�� �������� �ΰ��� �����尡 ���ư��鼭 count ���
// ������ :  gcc -o mutextest mutextest.c -lpthread -D_REENTRANT
// ���� :  mutextest
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// ������ ���� �Լ�
void *thrfunc(void *arg) ;
int counting = 0;                  // ����������
pthread_mutex_t count_lock;        // ���ؽ� ����
pthread_mutexattr_t mutex_attr;    // ���ؽ� �Ӽ� ����

int main(int argc, char **argv) {
    pthread_t tid[2];
    int i, status;
    // ���ؽ� �Ӽ� �������ʱ�ȭ�ϰ�, �Ӽ����� �⺻������ �����Ѵ�.
    pthread_mutexattr_init(&mutex_attr); 
    pthread_mutex_init(&count_lock, &mutex_attr); // ���ؽ� �ʱ�ȭ
    for(i=0; i <2; i++) {
        // ���� �߻��� non-zero �� ����
        if((status=pthread_create(&tid[i], NULL, &thrfunc, NULL))!=0) {
            printf("pthread_create fail: %s",strerror(status));
            exit(0);
        }
    }
    for(i=0; i<2; i++) pthread_join(tid[i], NULL);
    return 0;
}

// ������ ���� �Լ�
void *thrfunc(void *arg) {
    while(1) {
        pthread_mutex_lock( &count_lock );
        printf("\n[%ld ������] ���ؽ� ���\n", pthread_self());
        printf("[%ld ������] counting = %d\n", pthread_self(), counting);
        counting++;
        sleep(1);
        printf("[%ld ������] ���ؽ� ����\n", pthread_self());
        pthread_mutex_unlock(&count_lock);
    }
    return NULL;
}