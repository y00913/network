//--------------------------------------------------------------
// ���ϸ� : port_number.c
// ��  �� : �ý����� �ڵ����� ������ ��Ʈ��ȣ�� ����ϴ� ���α׷�
// ������ : cc -o port_number port_number.c
// ���� : port_number
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MSG "Test Message"

int main() {
    int sd1, sd2 ;
    int addrlen;
    struct sockaddr_in servaddr, cliaddr;  // �����ּ� ����ü
    unsigned short port1, port2;   // ��Ʈ��ȣ

    // �����ּ� ����ü �ʱ�ȭ
    servaddr.sin_family = AF_INET ;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY) ;//INADDR_ANY�� �ڵ����� �ڽ��� �ּҸ� find 
    servaddr.sin_port = htons(7);

    sd1=socket(PF_INET, SOCK_STREAM, 0);
    sd2=socket(PF_INET, SOCK_DGRAM, 0);

    // TCP ������ ��Ʈ��ȣ ���
    if( connect(sd1, (struct sockaddr*)&servaddr, sizeof(servaddr))<0) {
        perror("connect fail");
        exit(1);
    }
    getsockname(sd1, (struct sockaddr*)&cliaddr, &addrlen) ;
    port1 = ntohs(cliaddr.sin_port);
    // UDP ������ ��Ʈ��ȣ ���
    sendto(sd2, MSG, strlen(MSG), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    getsockname(sd2, (struct sockaddr*)&cliaddr, &addrlen) ;
    port2 = ntohs(cliaddr.sin_port);

    printf("��Ʈ��     ���� ��Ʈ��ȣ = %d\n", port1) ;
    printf("�����ͱ׷� ���� ��Ʈ��ȣ = %d\n", port2) ;
    close(sd1) ;
    close(sd2) ;
    return 0;
}