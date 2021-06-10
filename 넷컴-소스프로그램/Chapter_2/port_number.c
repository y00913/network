//--------------------------------------------------------------
// 파일명 : port_number.c
// 기  능 : 시스템이 자동으로 배정한 포트번호를 출력하는 프로그램
// 컴파일 : cc -o port_number port_number.c
// 사용법 : port_number
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
    struct sockaddr_in servaddr, cliaddr;  // 소켓주소 구조체
    unsigned short port1, port2;   // 포트번호

    // 소켓주소 구조체 초기화
    servaddr.sin_family = AF_INET ;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY) ;//INADDR_ANY는 자동으로 자신의 주소를 find 
    servaddr.sin_port = htons(7);

    sd1=socket(PF_INET, SOCK_STREAM, 0);
    sd2=socket(PF_INET, SOCK_DGRAM, 0);

    // TCP 소켓의 포트번호 얻기
    if( connect(sd1, (struct sockaddr*)&servaddr, sizeof(servaddr))<0) {
        perror("connect fail");
        exit(1);
    }
    getsockname(sd1, (struct sockaddr*)&cliaddr, &addrlen) ;
    port1 = ntohs(cliaddr.sin_port);
    // UDP 소켓의 포트번호 얻기
    sendto(sd2, MSG, strlen(MSG), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    getsockname(sd2, (struct sockaddr*)&cliaddr, &addrlen) ;
    port2 = ntohs(cliaddr.sin_port);

    printf("스트림     소켓 포트번호 = %d\n", port1) ;
    printf("데이터그램 소켓 포트번호 = %d\n", port2) ;
    close(sd1) ;
    close(sd2) ;
    return 0;
}