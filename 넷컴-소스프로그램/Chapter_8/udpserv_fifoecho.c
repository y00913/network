//------------------------------------------------------------
// ���ϸ� : udpserv_fifoecho.c
// ��  �� : FIFO�� �̿��Ͽ� ���� �޽����� ����
// ������ : gcc -o udpserv_fifoecho udpserv_fifoecho.c
// ��  �� : udpserv_pipeecho port
//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_BUFSZ  512
#define FIFONAME   "myfifo"

typedef struct mesg {
    struct sockaddr_in addr;       // Ŭ���̾�Ʈ �ּ�
    char data[ MAX_BUFSZ ];     // ���� ������
} mesg_t;

void child_start(int sock);           // �ڽ� ���μ���
void parent_start(int sock);         // �θ� ���μ���
void errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char **argv) {
    struct sockaddr_in servaddr;
    pid_t pid;
    int sock, port, len = sizeof(struct sockaddr);

    if(argc!=2) {
        printf("\n Usage : %s port\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0) {
        perror("socket failed");
          exit(EXIT_FAILURE);
    }

    bzero(&servaddr,len);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = ntohs(port);
    bind(sock, (struct sockaddr*)&servaddr, len);

    // FIFO ����
    if(mkfifo(FIFONAME,0660) ==-1 && errno!=EEXIST)
        errquit("mkfifo fail ");

    pid = fork();
    if (pid < 0 ) errquit("fork fail");
    else if(pid > 0) parent_start(sock);
    else if(pid ==0) child_start(sock);

    return 0;
}

// �ڽ� ���μ���
void child_start(int sock) {
    mesg_t pmsg;
    int nbytes, fiford, len = sizeof(struct sockaddr);

    // �б� ���� FIFO open
    fiford = open(FIFONAME,O_RDONLY);
    if(fiford==-1)
        errquit("fifo open fail ");

    while(1) {
        // �������κ��� �б� ���
        nbytes = read(fiford, (char *)&pmsg, sizeof(pmsg));
        if(nbytes<0)
            errquit("read failed ");

        printf("Child  : read from fifo\n");
        // �������κ��� ���� �����͸� Ŭ���̾�Ʈ�� ����
        nbytes = sendto(sock, &pmsg.data, strlen(pmsg.data),0, 
				(struct sockaddr*)&pmsg.addr, len);
        printf("Child  : %d bytes echo response\n", nbytes);
        printf("--------------------------------\n");
    }
}

// �θ� ���μ���
void parent_start(int sock) {
    mesg_t  pmsg;
    int nbytes, fifowd, len = sizeof(struct sockaddr);

    // ���� ���� FIFO open
    fifowd = open(FIFONAME, O_WRONLY);
    if(fifowd==-1)
        errquit("fifo open fail ");
    printf("my echo server wait...\n");
    while(1) {
        // �������κ��� �б� ���
        nbytes = recvfrom(sock, (void*)&pmsg.data, MAX_BUFSZ, 0,
				   (struct sockaddr*)&pmsg.addr,&len);
        if(nbytes < 0) errquit("recvfrom failed ");
        pmsg.data[nbytes]=0;
        printf("Parent : %d bytes recv from socket\n",nbytes);

        // �������κ��� ���� �����͸� FIFO�� ����
        if( write(fifowd, &pmsg, sizeof(pmsg))<0)
            perror("write fail ");
        printf("Parent : write to fifo\n");
    }
}