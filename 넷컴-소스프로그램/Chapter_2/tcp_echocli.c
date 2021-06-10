//--------------------------------------------------------------
// 파일명 : tcp_echocli.c
// 기  능 : 에코 서비스를 요청하는 TCP(연결형) 클라이언트
// 컴파일 : cc -o tcp_echocli tcp_echocli.c
// 사용법 : tcp_echocli 203.252.65.3
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE  127

int main(int argc, char *argv[]) {
    struct sockaddr_in servaddr;
    int  s, nbyte;
    char buf[MAXLINE+1];

    if(argc != 2) {
        printf("usage: %s ip_address\n", argv[0]);
        exit(0);
    }

    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(0);
    }
    // 에코 서버의 소켓주소 구조체 작성
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_port = htons(7);

    // 연결요청
    if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect fail");
            exit(0);
    }

    printf("입력: ");
    if (fgets(buf, sizeof(buf), stdin) == NULL)
        exit(0);
    nbyte = strlen(buf);
    // 에코 서버로 메시지 송신
    if (write(s, buf, nbyte) < 0) {
        printf("write error\n");
        exit(0);
    }
    // 수신된 에코 데이터 화면출력
    printf("수신 : ");
    if( (nbyte=read(s, buf, MAXLINE)) <0) {
        perror("read fail");
        exit(0);
    }
    buf[nbyte]=0;
    printf("%s", buf);

    close(s);
    return 0;
}
