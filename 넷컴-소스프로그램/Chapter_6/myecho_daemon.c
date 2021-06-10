//--------------------------------------------------------------
// ���ϸ� : myecho_daemon.c
// ��  �� : tcp_echoserv.c�� ���� ���񽺸� �������� ����
// ������ : cc -o myecho_daemon myecho_daemon -lsocket
// ���� : myecho_daemon 6000
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <syslog.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLINE     511
#define MAXFD       64

int  tcp_listen(int host, int port, int backlog);

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr;
    int listen_sock, accp_sock; // ���� ��ȣ
    int i, addrlen, nbyte;
    pid_t pid;
    char buf[MAXLINE+1];
    struct sigaction sact;

    if (argc != 2) {
        printf("����: %s port\n", argv[0]);
        exit(0);
    }

    // ���α׷��� ���� ���μ����� �����Ű�� �ڵ�
    if((pid = fork()) != 0)
        exit(0);// �θ� ���μ����� �����Ų��
              // ��, �ڽ� ���μ����� �Ʒ� �κ��� �����Ѵ�
    setsid();   // ������ ���� ������ �ȴ�

    // SIGHUP �ñ׳�(hang up)�� �����Ѵ�
    sact.sa_handler = SIG_IGN;
    sact.sa_flags = 0;
    sigemptyset(&sact.sa_mask);
    sigaddset(&sact.sa_mask, SIGHUP);
    sigaction(SIGHUP, &sact,NULL);

    if ((pid = fork()) != 0)// �ٽ� �ڽ� ���μ���(����)�� �����
        exit(0);          // �θ� ���μ����� �����Ų��
    chdir ("/");           // ���丮 ����
    umask(0);           // ���� ������� ���ϰ� ���丮���� ����Ʈ ������ ���� ���� �� ���
    for (i = 0; i < MAXFD ; i++)
        close(i);    // Ȥ�� �����Ǿ� ������ �𸣴� ������ �ݴ´�

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]),5);
    // interactive ���� ���� ����
    while(1) {
        addrlen = sizeof(cliaddr);
        // �����û�� ��ٸ�
        if ((accp_sock = accept(listen_sock,(struct sockaddr *)&cliaddr, &addrlen)) < 0)
          exit(0);
        if ((nbyte = read(accp_sock, buf, MAXLINE)) < 0)
            exit(0);
        write(accp_sock, buf, nbyte); // ����
        close(accp_sock);
    }
    close(listen_sock);
}

// listen ���� ���� �� listen
int  tcp_listen(int host, int port, int backlog) {
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1) {
        perror("socket fail");
        exit(1);
    }
    // servaddr ����ü�� ���� ����
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);
    if (bind(sd , (struct sockaddr *)&servaddr,  sizeof(servaddr)) <
                        0) {
        perror("bind fail");    exit(1);
    }
    // ������ ���� ��ȯ
    listen(sd, backlog);
    return sd;
}