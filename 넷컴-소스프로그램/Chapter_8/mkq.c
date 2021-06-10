//--------------------------------------------------------------
// ���ϸ� : mkq.c
// ��  �� : �޽���ť�� �����ϰ� �޽���ť ID�� ���
// ������ : gcc -o mkq mkq.c
// ��  �� : mkq key_value
//--------------------------------------------------------------

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int qid;
    key_t key;

    if(argc!=2) {
        printf("Usage : %s  key\n",argv[0]);
        exit(1);
    }

    key = atoi(argv[1]);
    // �޽���ť ����
    if((qid=msgget(key, IPC_CREAT | IPC_EXCL | 0666))<0) {
        perror("msgget fail");
        exit(EXIT_FAILURE);
    }

    // �޽���ť�� ID ���
    printf("created queue id = %d\n", qid);

    // key ������ �̹� ������ �޽���ť ID ���
    qid = msgget(key, 0);
    printf("key is  = %d\n", key);
    printf("opened queue id = %d\n", qid);

    return 0;
}