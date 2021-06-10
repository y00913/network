//--------------------------------------------------------------
// 파일명 : mydaytime.c
// 기  능 : daytime 서비스를 요청하는 TCP(연결형) 클라이언트
// 컴파일 : cc -o mydaytime mydaytime.c
// 사용법 : mydaytime 203.252.65.3
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

    // 소켓 생성
    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(1);
    }
    // 서버의 소켓주소 구조체 servaddr을 '\0'으로 초기화
    bzero((char *)&servaddr, sizeof(servaddr));
    // servaddr의 주소 지정
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);// dotted decimal -> IP 주소
    servaddr.sin_port = htons(13);

    // 연결요청
    if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect fail");
        exit(1);
    }
    // 서버가 보내오는 daytime 데이터의 수신 및 화면출력
    if((nbyte=read(s, buf, MAXLINE )) < 0) {
        perror("read fail");
        exit(1);
    }
    buf[nbyte] = 0;
    printf("%s", buf);
    close(s);
    return 0;
}