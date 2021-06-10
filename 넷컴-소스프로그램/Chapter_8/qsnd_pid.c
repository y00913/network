//--------------------------------------------------------------
// ���ϸ� : qsnd_pid.c
// ��  �� : ������ ���μ������� �޽��� ����
// ������ : gcc -o qsnd_pid qsnd_pid.c
// ��  �� : qsnd_pid key
//--------------------------------------------------------------

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSZ 512

// �޽��� ���� ����ü ����
typedef struct _msg {
    long msg_type;
    char msg_text[BUFSZ];
} msg_t ;

int main(int argc, char **argv) {
    pid_t pid;
    int len, qid;
    msg_t pmsg;		//������ �޽���
    key_t key;          // �޽���ť key

    if(argc !=2) {
        printf("Usage : %s  msqkey",argv[0]);
        exit(EXIT_FAILURE);
    }

    key =  atoi(argv[1]);

    // �޽���ť ����
    if((qid=msgget(key,IPC_CREAT | 0600))<0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if((pid=fork()) < 0) {
        printf("fork fail ");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0) {
        // �θ� ���μ���
        int nbytes;
        msg_t rmsg;
        printf("\n%d ���μ��� �޽���ť �б� �����..\n", getpid());
        nbytes = msgrcv(qid, &rmsg, BUFSZ, getpid(), 0);
        printf("recv = %d bytes \n", nbytes);
        printf("type = %ld \n", rmsg.msg_type);
        printf("���� ���μ��� PID= %d \n", getpid());
        printf("value = %s \n", rmsg.msg_text);
        msgctl(qid, IPC_RMID, 0);        // �޽���ť ����
        exit(EXIT_SUCCESS);
    }

    // �ڽ� ���μ���
    puts("Enter message to post : ");
    if( fgets((&pmsg)->msg_text, BUFSZ, stdin)==NULL) {
        puts("no message to post");
        exit(EXIT_FAILURE);
    }
    pmsg.msg_type = getppid();  // �θ� ���μ����� ID�� �޽��� Ÿ��
    len=strlen(pmsg.msg_text);

    // �޽��� ����
    if((msgsnd(qid, &pmsg, len, 0))<0 ) {
        perror("msgsnd fail");
        exit(EXIT_FAILURE);
    }
    puts("message posted");

    return ;
}