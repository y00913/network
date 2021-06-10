//--------------------------------------------------------------
// ���ϸ� : catch_sigint4.c
// ��  �� : SIGINT �ñ׳ο� ���� �ڵ鷯 ����ϴ� ��
// ������ : gcc -o catch_sigint1  catch_sigint1.c
// ��  �� : catch_sigint4
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int count;

// �ñ׳� �ڵ鷯
void catch_sigint(int  signum) {
    static int call_count = 0; // ���� ���� ���
    printf("catch_sigint %d��° ȣ�� \n", call_count++);
    sleep(3);
    printf("catch_sigint ����\n");
    return;
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    sigset_t masksets;
    int n;
    char buf[10];
    sigfillset(&masksets);

    // �ñ׳� �ڵ鷯 ��ġ
    act.sa_handler = catch_sigint;

    // �ñ׳� �ڵ鷯�� ����Ǵ� ���� ��� �ñ׳��� �����
    act.sa_mask = masksets;
    act.sa_flags = SA_RESTART | SA_NODEFER;
    sigaction(SIGINT, &act, NULL);// �ý��� �ڵ鷯 ���

    for(count=0; count<3 ; count++ ) {
       n = read(0, buf, sizeof(buf));
       if(n==-1 && errno ==EINTR)
	   printf("read �Լ� Interrrupted\n");
       else
	   printf("\t** %d byte input\n", n);
    }
    return 0;
}