//--------------------------------------------------------------
// ���ϸ� : tcp_talkserv.c
// ��  �� : ��ũ Ŭ���̾�Ʈ�� 1:1 ����� �Ѵ�
// ������ : cc -o tcp_talkserv tcp_talkserv.c
// ���� : tcp_talkserv 3000
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char *EXIT_STRING = "exit"; // ���Ṯ��
int recv_and_print(int sd); // ���κ��� �޽��� ������ ȭ�����, �ڽ� ���μ������� ����
int input_and_send(int sd); // Ű�����Է¹ް� ��뿡�� �޽�������, �θ� ���μ������� ����

#define MAXLINE 511

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr, servaddr;
    int listen_sock, accp_sock, addrlen = sizeof(cliaddr); // ���Ϲ�ȣ, �ּұ���
    pid_t pid;
    if (argc != 2) {
        printf("����: %s port\n", argv[0]);
        exit(0);
    }
    // ���� ����
    if((listen_sock=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(0);
    }
    // ������ �����ּ� ����ü servaddr�� '0'���� �ʱ�ȭ
    bzero((char *)&servaddr, sizeof(servaddr));
    // servaddr�� ����
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    // bind() ȣ��
    if(bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");
        exit(0);
    }
    puts("������ Ŭ���̾�Ʈ�� ��ٸ��� �ֽ��ϴ�");
    listen(listen_sock, 1);
    // Ŭ���̾�Ʈ �����û ����
    if((accp_sock = accept(listen_sock,(struct sockaddr *)&cliaddr, &addrlen)) < 0) {
        perror("accept fail");
        exit(0);
    }
    puts("Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.");
    if( (pid = fork()) > 0)
        input_and_send(accp_sock); // Ű�����Է¹ް� ��뿡�� �޽�������
    else if (pid == 0)
        recv_and_print(accp_sock); // ���κ��� �޽��� ������ ȭ�����
    close(listen_sock);
    close(accp_sock);
    return 0;
}

// Ű�����Է¹ް� ��뿡�� �޽�������
int input_and_send(int sd) {
    char buf[MAXLINE+1];
    int nbyte;
    while(fgets(buf, sizeof(buf), stdin) != NULL) {
        nbyte = strlen(buf);
        write(sd, buf, strlen(buf));
        // ���Ṯ�ڿ� �Է� ó��
        if (strstr(buf, EXIT_STRING) != NULL ) {
            puts("Good bye.");
            close(sd);
            exit(0);
        }
    }
    return 0;
}

// ���κ��� �޽��� ������ ȭ�����
int recv_and_print(int sd) {
    char buf[MAXLINE+1];
    int nbyte;
    while(1) {
        if( (nbyte=read(sd, buf, MAXLINE))<0){
            perror("read fail");
            close(sd);
            exit(0);
        }
        buf[nbyte] = 0;
        // ���Ṯ�ڿ� ���Ž� ����
        if (strstr(buf, EXIT_STRING) != NULL )
            break;
        printf("%s", buf); // ȭ�� ���
    }
    return 0;
}