//-------------------------------------------------------------
// 파일명 : tcp_chatserv_nonb.c
// 기  능 : 넌블록 모드의 채팅 서버
// 컴파일 : cc -o tcp_chatserv_nonb tcp_chatserv_nonb
// 사용법 : tcp_chatserv_nonb  4001
//-------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE  511
#define MAX_SOCK 1024 //솔라리스는 64

char *EXIT_STRING = "exit";
char *START_STRING = "Connected to chat_server\n";
int maxfdp1;                // 최대 소켓번호 +1
int num_chat = 0;           // 채팅 참가자 수
int clisock_list[ MAX_SOCK ]; // 채팅에 참가자 소켓번호 목록
int listen_sock;

// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr);
void removeClient(int);        // 채팅 탈퇴 처리 함수
int set_nonblock(int sockfd);  // 소켓을 넌블록으로 설정
int is_nonblock(int sockfd);   // 소켓이 넌블록 모드인지 확인
int tcp_listen(int host, int port, int backlog); // 소켓 생성 및 listen
void   errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    int i, j, nbyte, count;
    int accp_sock, clilen;
    struct sockaddr_in  cliaddr;

    if(argc != 2) {
        printf("사용법 :%s port\n", argv[0]);
       exit(0);
   }

    listen_sock = tcp_listen(INADDR_ANY,atoi(argv[1]),5);
    if(listen_sock==-1)
        errquit("tcp_listen fail");
    if(set_nonblock(listen_sock) == -1)
        errquit("set_nonblock fail");

    for(count=0; ;count++) {
        if(count==100000) {
           putchar('.' );
           fflush(stdout); count=0;
        }
        addrlen = sizeof(cliaddr);
        accp_sock= accept(listen_sock, (struct sockaddr *) &cliaddr, &clilen);
        if(accp_sock == -1 && errno!=EWOULDBLOCK)
            errquit("accept fail");
        else if(accp_sock >0) {
            // 채팅 클라이언트 목록에 추가
            //clisock_list[num_chat] = accp_sock;
            // 통신용 소켓은 넌블록 모드가 아님
            if(is_nonblock(accp_sock)!=0 && set_nonblock(accp_sock)<0 )
                errquit("set_nonblock fail");
            addClient(accp_sock,&cliaddr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("%d번째 사용자 추가.\n", num_chat);
        }

        // 클라이언트가 보낸 메시지를 모든 클라이언트에게 방송
        for(i = 0; i < num_chat; i++) {
            errno = 0;
            nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
            if(nbyte==0) { // abrupt exit(reset, ctrl-C)
                removeClient(i);    
                continue;
            }
            else if( nbyte==-1 && errno == EWOULDBLOCK) //보낸 데이터 없음, 즉시 리턴
                continue;
            // 종료문자 처리
            if(strstr(buf, EXIT_STRING) != NULL) {
                removeClient(i);    // abrupt exit
                continue;
            }
            // 모든 채팅 참가자에게 메시지 방송
            buf[nbyte] = 0;
            for (j = 0; j < num_chat; j++)
                send(clisock_list[j], buf, nbyte, 0);
            printf("%s\n", buf);
        }
    }
}

// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr) {
    char buf[20];
    inet_ntop(AF_INET,&newcliaddr->sin_addr,buf,sizeof(buf));
    printf("new client: %s\n",buf);
     // 채팅 클라이언트 목록에 추가
    clisock_list[num_chat] = s;
    num_chat++;
}
    // 채팅 탈퇴 처리
void removeClient(int i) {
    close(clisock_list[i]);
    if(i != num_chat-1)
        clisock_list[i] = clisock_list[num_chat-1];
    num_chat--;
    printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_chat);
}

// 소켓이 nonblock 인지 확인
int is_nonblock(int sockfd) {
    int val;
    // 기존의 플래그 값을 얻어온다
    val=fcntl(sockfd, F_GETFL,0);
    // 넌블록 모드인지 확인
    if(val & O_NONBLOCK)
        return 0;
    return -1;
}

    // 소켓을 넌블록 모드로 설정
int set_nonblock(int sockfd) {
    int val;
    // 기존의 플래그 값을 얻어온다
    val=fcntl(sockfd, F_GETFL,0);
    if(fcntl(sockfd, F_SETFL, val | O_NONBLOCK) == -1)
        return -1;
    return 0;
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
    if (bind(sd , (struct sockaddr *)&servaddr, sizeof(servaddr))
        < 0) {
        perror("bind fail");  exit(1);
    }
    // 클라이언트로부터 연결요청을 기다림
    listen(sd, backlog);
    return sd;
}
