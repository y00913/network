//--------------------------------------------------------------
// 파일명 : tcp_chatcli.c
// 기  능 : 서버에 접속한 후 키보드의 입력을 서버로 전달하고,
//          서버로부터 오는 메시지를 화면에 출력한다
// 컴파일 : cc -o tcp_chatcli tcp_chatcli.c
// 사용법 : chat_client 203.252.65.3 4001 name
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE     1000
#define NAME_LEN    20

char *EXIT_STRING= "exit";
// 소켓 생성 및 서버 연결, 생성된 소켓리턴
int tcp_connect(int af, char *servip, unsigned short port);
void errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
    char bufall[MAXLINE+NAME_LEN], // 이름+메시지를 위한 버퍼
         *bufmsg;  // bufall 에서 메시지 시작 부분의 포인터
    int maxfdp1,   // 최대 소켓 디스크립터
       s,          // 소켓
       namelen;   // 이름의 길이
    fd_set read_fds;

    if(argc != 4) {
        printf("사용법 : %s sever_ip  port name \n", argv[0]);
        exit(0);
    }

    sprintf(bufall, "[%s] :", argv[3]);  // bufall 의 앞부분에 이름을 저장
    namelen= strlen(bufall);
    bufmsg = bufall+namelen;  // 메시지 시작 부분 지정

    s = tcp_connect(AF_INET, argv[1], atoi(argv[2]));
    if(s==-1)
        errquit("tcp_connect fail");

    puts("서버에 접속되었습니다.");
    maxfdp1 = s + 1;
    FD_ZERO(&read_fds);

    while(1) {
        FD_SET(0, &read_fds);   // 표준입력
        FD_SET(s, &read_fds);   // 통신소켓
        if(select(maxfdp1, &read_fds, NULL,NULL,NULL) < 0)
            errquit("select fail");
        if (FD_ISSET(s, &read_fds)) { // 통신소켓
            int nbyte;
            if ((nbyte = recv(s, bufmsg, MAXLINE, 0)) > 0)  {
                bufmsg[nbyte] = 0;
                printf("%s \n", bufmsg);
            }
        }
        if (FD_ISSET(0, &read_fds)) { // 표준 입력
            if(fgets(bufmsg, MAXLINE, stdin)) {
                if (send(s, bufall, namelen+strlen(bufmsg), 0) < 0)
                    puts("Error : Write error on socket.");
                if (strstr(bufmsg, EXIT_STRING) != NULL ) {
                    puts("Good bye.");
                    close(s);
                    exit(0);
                }
            }
        }
    } // end of while
}

int tcp_connect(int af, char *servip, unsigned short port) {
    struct sockaddr_in servaddr;
    int  s;
    // 소켓 생성
    if ((s = socket(af, SOCK_STREAM, 0)) < 0)
        return -1;
    // 채팅 서버의 소켓주소 구조체 servaddr 초기화
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = af;
    inet_pton(AF_INET, servip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    // 연결요청
    if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        return -1;
    return s;
}
