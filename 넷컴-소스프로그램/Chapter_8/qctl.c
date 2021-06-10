//--------------------------------------------------------------
// ���ϸ� : qctl
// ��  �� : msgctl()�� �̿��� �޽���ť�� ������ ���
// ������ : gcc -o qctl  qctl.c
// ��  �� : qctl  msqkey
//--------------------------------------------------------------

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int msq_remove(int qid);	// �޽���ť ���� �Լ�
int view_qinfo(int qid);        // �޽���ť ������� �Լ�

int main(int argc, char **argv) {
    int qid;
    key_t key;

    if(argc != 2) {
        printf("USAGE : %s   msqkey\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    key = atoi(argv[1]);

    // ������ ������ �޽���ť ID ���
    if((qid=msgget(key, 0)) < 0) {
      perror("msgget fail ");
      exit(0);
    }

    view_qinfo(qid);              // �޽���ť ���� ���
    msq_remove(qid);              // �޽���ť ����
    exit(EXIT_SUCCESS);
}

// �޽���ť ���� ����Լ�
int view_qinfo(int qid) {
    struct msqid_ds  buf;
    struct ipc_perm  *pm;

    // �޽���ť ��ü ���
    if((msgctl(qid, IPC_STAT, &buf))<0) {
        perror("msgctl");
        return -1;
    }
    pm = &buf.msg_perm;
    printf("ť�� �ִ� ����Ʈ�� : %d\n",buf.msg_qbytes);
    printf("ť�� ��ȿ ����� UID : %d\n",pm->uid);
    printf("ť�� ��ȿ ����� GID : %d\n",pm->gid);
    printf("ť ���ٱ��� : 0%o\n",pm->mode);
    return 0;
}

// �޽���ť ���� �Լ�
int msq_remove(int qid) {
    if((msgctl(qid, IPC_RMID, NULL)) < 0) {
        perror("msgctl");
        return -1;
    }
    printf("�޽���ť %d ������\n", qid);
    return 0;
}