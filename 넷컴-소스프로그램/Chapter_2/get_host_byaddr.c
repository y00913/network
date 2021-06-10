//--------------------------------------------------------------
// ���ϸ� : get_host_byaddr.c
// ��  �� : hostent ����ü ���� ��� ���α׷�
// ������ : cc -o get_host_byaddr get_host_byaddr.c
// ���� : get_host_byaddr 192.203.144.27
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    struct hostent   *myhost;
    struct in_addr   in;

    if(argc < 2) {
        printf("���� : %s ip_address \n", argv[0]);
        exit(0);
    }

    inet_pton(AF_INET, argv[1], &in.s_addr);// dotted decimal-> 32bit �ּ�
    myhost = gethostbyaddr((char *)&(in.s_addr),sizeof(in.s_addr), AF_INET);

    if (myhost == NULL) {
        printf("Error at gethostbyaddr() \n");
        exit(0);
     }
     printf("ȣ��Ʈ �̸� : %s\n", myhost->h_name);
     return 0;
}