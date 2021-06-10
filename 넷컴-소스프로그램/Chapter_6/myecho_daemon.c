//--------------------------------------------------------------
// 파일명 : myecho_daemon.c
// 기  능 : tcp_echoserv.c의 에코 서비스를 데몬으로 실행
// 컴파일 : cc -o myecho_daemon myecho_daemon -lsocket
// 사용법 : myecho_daemon 6000
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <syslog.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLINE     511
#define MAXFD       64

int  tcp_listen(int host, int port, int backlog);

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr;
    int listen_sock, accp_sock; // 소켓 번호
    int i, addrlen, nbyte;
    pid_t pid;
    char buf[MAXLINE+1];
    struct sigaction sact;

    if (argc != 2) {
        printf("사용법: %s port\n", argv[0]);
        exit(0);
    }

    // 프로그램을 데몬 프로세스로 실행시키는 코드
    if((pid = fork()) != 0)
        exit(0);// 부모 프로세스는 종료시킨다
              // 즉, 자식 프로세스만 아래 부분을 실행한다
    setsid();   // 스스로 세션 리더가 된다

    // SIGHUP 시그널(hang up)을 무시한다
    sact.sa_handler = SIG_IGN;
    sact.sa_flags = 0;
    sigemptyset(&sact.sa_mask);
    sigaddset(&sact.sa_mask, SIGHUP);
    sigaction(SIGHUP, &sact,NULL);

    if ((pid = fork()) != 0)// 다시 자식 프로세스(손자)를 만든다
        exit(0);          // 부모 프로세스는 종료시킨다
    chdir ("/");           // 디렉토리 변경
    umask(0);           // 새로 만들어질 파일과 디렉토리들의 디폴트 소유권 값을 정할 때 사용
    for (i = 0; i < MAXFD ; i++)
        close(i);    // 혹시 개설되어 있을지 모르는 소켓을 닫는다

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]),5);
    // interactive 에코 서비스 수행
    while(1) {
        addrlen = sizeof(cliaddr);
        // 연결요청을 기다림
        if ((accp_sock = accept(listen_sock,(struct sockaddr *)&cliaddr, &addrlen)) < 0)
          exit(0);
        if ((nbyte = read(accp_sock, buf, MAXLINE)) < 0)
            exit(0);
        write(accp_sock, buf, nbyte); // 전송
        close(accp_sock);
    }
    close(listen_sock);
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
    if (bind(sd , (struct sockaddr *)&servaddr,  sizeof(servaddr)) <
                        0) {
        perror("bind fail");    exit(1);
    }
    // 수동형 모드로 변환
    listen(sd, backlog);
    return sd;
}