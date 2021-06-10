//--------------------------------------------------------------
// ���ϸ� : mydaytime.c
// ��  �� : daytime ���񽺸� ��û�ϴ� TCP(������) Ŭ���̾�Ʈ
// ������ : cc -o mydaytime mydaytime.c
// ���� : mydaytime 203.252.65.3
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 127

int main(int argc, char *argv[]) {
    int s, nbyte;
    struct  sockaddr_in servaddr;
    char buf[ MAXLINE+1 ];

    if(argc != 2) {
        printf("Usage: %s ip_address\n", argv[0]);
        exit(0);
    }

    // ���� ����
    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(1);
    }
    // ������ �����ּ� ����ü servaddr�� '\0'���� �ʱ�ȭ
    bzero((char *)&servaddr, sizeof(servaddr));
    // servaddr�� �ּ� ����
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);// dotted decimal -> IP �ּ�
    servaddr.sin_port = htons(13);

    // �����û
    if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect fail");
        exit(1);
    }
    // ������ �������� daytime �������� ���� �� ȭ�����
    if((nbyte=read(s, buf, MAXLINE )) < 0) {
        perror("read fail");
        exit(1);
    }
    buf[nbyte] = 0;
    printf("%s", buf);
    close(s);
    return 0;
}