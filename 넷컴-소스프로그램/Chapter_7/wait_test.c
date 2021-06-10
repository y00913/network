//--------------------------------------------------------------
// ���ϸ� : wait_test.c
// ��  �� : wait() �Լ� ������ ����
// ������ : gcc -o wait_test wait_test.c
// ��  �� : wait_test
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// ����� �ñ׳� �ڵ鷯 �Լ�
void catch_sigchld(int signo) { puts("###( Parent ) catch SIGCHLD "); }

int chstat;                  // ������� ��
int main(int argc, char *argv[]) {
    int i,n;
    struct sigaction sact;
    sact.sa_flags = 0;
    sigemptyset(&sact.sa_mask);
    sigaddset(&sact.sa_mask, SIGCHLD);

    // �ñ׳� �ڵ鷯 ���
    sact.sa_handler = SIG_DFL;       // �⺻����(����)
    //sact.sa_handler = SIG_IGN;      // �ñ׳� ���� ���
    //sact.sa_handler = catch_sigchld;// ����� �ڵ鷯 ���
    sigaction(SIGCHLD, &sact, NULL);

    // �ڽ� ���μ��� ����
    for(i=0; i<5; i++) {
        if(fork()==0) {
            if(i>2) sleep(6); // �θ��� wait() ȣ�⺸�� �ʰ� ����
            printf("(%d�� Child),PID=%d,PPID=%d Exited\n",i,getpid(),getppid());
            exit(13); // ���ᰪ 13��
        }
    }
    // �θ� ���μ���
    sleep(3); // 0,1,2 �� �ڽ��� �����ϱ⸦ ��ٸ�
    puts("------------------");
    system("ps -a"); // ���� �ý����� ���μ��� ����Ȯ��
    puts("------------------");

    puts("#( Parent ) wait ȣ����");
    for( ; ; ) {
        chstat = -1; // �ʱ�ȭ
        n = wait(&chstat);
        printf("# wait = %d(child stat=%d)\n",n, chstat);
        if( n == -1) {
            if(errno ==ECHILD) {
                perror("��ٸ� �ڽ����μ����� �������� ����"); break;
            }
            else if(errno == EINTR) {
                perror("wait �ý��� ���� ���ͷ�Ʈ ��"); continue;
            }
        }
    }
    puts("#( Parent ) ������");
    return 0;
}