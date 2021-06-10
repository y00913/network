//--------------------------------------------------------------
// ���ϸ� : catch_sigint1.c
// ��  �� : SIGINT �ñ׳ο� ���� �ڵ鷯 ����ϴ� ��
// ������ : gcc -o catch_sigint1  catch_sigint1.c
// ��  �� : catch_sigint1
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
    printf("\n(count=%d)  CTRL-C pressed!\n", count);
    return;
}

int main(int argc, char *argv[]) {
    struct sigaction act;
    sigset_t masksets;
    int i;
    char buf[10];
    sigfillset(&masksets);

    // �ñ׳� �ڵ鷯 ��ġ
    act.sa_handler = catch_sigint;

    // �ñ׳� �ڵ鷯�� ����Ǵ� ���� ��� �ñ׳��� �����
    act.sa_mask = masksets;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);// �ý��� �ڵ鷯 ���

    for(count=0; count<3 ; count++ )
       read(0, buf, sizeof(buf));
    return 0;
}