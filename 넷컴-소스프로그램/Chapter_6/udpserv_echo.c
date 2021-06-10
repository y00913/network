//--------------------------------------------------------------
// 파일명 : udpserv_echo.c
// 동  작 : Xinetd에 등록되어 UDP 에코 요청을 처리함
// 컴파일 : gcc -o myecho_udpserv  udpserv_echo.c
// 실  행 : 에코 메시지가 들어오면 Xinetd에 의해 자동 실행됨
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