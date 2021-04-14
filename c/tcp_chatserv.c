//-------------------------------------------------------------
// 파일명 : tcp_chatserv.c
// 기  능 : 채팅 참가자 관리, 채팅 메시지 수신 및 방송
// 컴파일 : cc -o chat_server tcp_chatserv.c
// 사용법 : chat_server 4001
//-------------------------------------------------------------
// 1:n 연결

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE  511
#define MAX_SOCK 1024 // 솔라리스의 경우 64

char *EXIT_STRING = "exit";   // 클라이언트의 종료요청 문자열
// 클라이언트 환영 메시지
char *START_STRING = "Connected to chat_server \n";
int maxfdp1;                // 최대 소켓번호 +1
int num_chat = 0;          // 채팅 참가자 수
int clisock_list[MAX_SOCK]; // 채팅에 참가자 소켓번호 목록
int listen_sock;             // 서버의 리슨 소켓

void addClient(int s, struct sockaddr_in *newcliaddr);// 새로운 채팅 참가자 처리
int getmax();               // 최대 소켓 번호 찾기
void removeClient(int s);    // 채팅 탈퇴 처리 함수
int tcp_listen(int host, int port, int backlog); // 소켓 생성, bind, listen
void errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr;
    char buf[MAXLINE+1];
    int i, j, nbyte, accp_sock, addrlen = sizeof(struct sockaddr_in);
    fd_set read_fds;  // 읽기를 감지할 fd_set 구조체

    if(argc != 2) {
        printf("사용법 :%s port\n", argv[0]);
        exit(0);
    }

    // tcp_listen(host, port, backlog) 함수 호출
    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
    while(1) {
        FD_ZERO(&read_fds);
        FD_SET(listen_sock, &read_fds);
        for(i=0; i<num_chat; i++)
            FD_SET(clisock_list[i], &read_fds);
        maxfdp1 = getmax() + 1;     // maxfdp1 재 계산
        puts("wait for client");
        if(select(maxfdp1, &read_fds,NULL,NULL,NULL)<0)
            errquit("select fail");

        if(FD_ISSET(listen_sock, &read_fds)) {//new client
           accp_sock=accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
            if(accp_sock == -1)
                errquit("accept fail");
            addClient(accp_sock, &cliaddr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("%d번째 사용자 추가.\n", num_chat);
        }

        // 클라이언트가 보낸 메시지를 모든 클라이언트에게 방송
        for(i = 0; i < num_chat; i++) {
           if(FD_ISSET(clisock_list[i], &read_fds)) {
                nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
                if(nbyte<= 0) { // 비정성적으로 종료(ctrl-C)
                    removeClient(i);    
                    continue;
                }
                buf[nbyte] = 0;
                // 종료문자 처리
                if(strstr(buf, EXIT_STRING) != NULL) {
                    removeClient(i);    // 클라이언트의 종료
                    continue;
                }
                // 모든 채팅 참가자에게 메시지 방송
                for (j = 0; j < num_chat; j++)
                    send(clisock_list[j], buf, nbyte, 0);
                printf("%s\n", buf);
            }
        }
    }  // end of while
    return 0;
}

// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr) {
    char buf[20];
    // 클라이언트 주소를 dotted decimal로 변환
    inet_ntop(AF_INET,&newcliaddr->sin_addr,buf,sizeof(buf));
    printf("new client: %s\n",buf);
     // 채팅 클라이언트 목록에 추가
    clisock_list[num_chat] = s;
    num_chat++;
}

// 채팅 탈퇴 처리
void removeClient(int s) {
    close(clisock_list[s]);
    if(s != num_chat-1)
        clisock_list[s] = clisock_list[num_chat-1];
    num_chat--;
    printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_chat);
}

// 최대 소켓번호 찾기
int getmax() {
    // Minimum 소켓번호는 가정 먼저 생성된 listen_sock
    int max = listen_sock;
    int i;
    for (i=0; i < num_chat; i++)
        if (clisock_list[i] > max )
            max = clisock_list[i];
    return max;
}

// listen 소켓 생성 및 listen
int  tcp_listen(int host, int port, int backlog) {
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1) {
        perror("socket fail");
        exit(1);
    }
    // servaddr 구조체의 내용 세팅
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);
    if (bind(sd , (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");  exit(1);
    }
    // 클라이언트로부터 연결요청을 기다림
    listen(sd, backlog);
    return sd;
}
