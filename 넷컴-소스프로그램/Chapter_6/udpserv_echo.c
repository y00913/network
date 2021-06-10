//--------------------------------------------------------------
// ���ϸ� : udpserv_echo.c
// ��  �� : Xinetd�� ��ϵǾ� UDP ���� ��û�� ó����
// ������ : gcc -o myecho_udpserv  udpserv_echo.c
// ��  �� : ���� �޽����� ������ Xinetd�� ���� �ڵ� �����
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in   peer;
    int rc,
       len = sizeof(struct sockaddr),
       pidsz;
    char buf[120];
    FILE *file;

    pidsz = sprintf(buf, "%d: ", getpid());
    rc = recvfrom(0, buf+pidsz, sizeof(buf)-pidsz, 0, (struct sockaddr *)&peer, &len);

    if(rc < 0) exit(1);
    sendto(1, buf, rc+pidsz, 0, (struct sockaddr *)&peer, len);
    exit(0);
}