//--------------------------------------------------------------
// ���ϸ� : udp_echocli.c
// ��  �� : ���� ���񽺸� ��û�ϴ� UDP(�񿬰���) Ŭ���̾�Ʈ
// ������ : cc -o udp_echocli  udp_echocli.c
// ���� : udp_echocli  203.252.65.3 9999
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE  511

int main(int argc, char *argv[]) {
    struct sockaddr_in servaddr;
    int s, nbyte, addrlen = sizeof(servaddr);
    char buf[MAXLINE+1];

    if(argc != 3) {
        printf("usage: %s ip_address port_number\n", argv[0]);
        exit(0);
    }

    if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket fail");
       exit(0);
    }
    // ���� ������ �����ּ� ����ü �ۼ�
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);//inet_pton()�� ����
    servaddr.sin_port = htons(atoi(argv[2]));
    // Ű���� �Է��� ����
    printf("�Է� : ");
    if (fgets(buf, MAXLINE , stdin)==NULL) {
        printf("fgets ����");
        exit(0);
    }
    // ���� ������ �޽��� �۽�
   if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, addrlen) < 0) {
        perror("sendto fail");
        exit(0);
    }
    // ���ŵ� ���� �޽���
    if ( (nbyte=recvfrom(s, buf, MAXLINE, 0,
                (struct sockaddr *)&servaddr, &addrlen)) < 0) {
        perror("recevfrom fail");
        exit(0);
    }
    buf[ nbyte ]=0;
    printf("%s\n", buf);
    close(s);
    return 0;
}