//------------------------------------------------------------
// 파일명 : udpserv_pipeecho.c
// 동  작 : 파이프를 통해 에코 메시지를 전달하는 UDP 에코 서버
// 컴파일 : gcc -o udpserv_pipeecho udpserv_pipeecho.c
// 실  행 : udpserv_pipeecho port
//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#define MAX_BUFSZ  512

// 파이프에 쓰는 데이터 구조
typedef struct mesg {
    struct sockaddr_in addr;    // 클라이언트 주소
    char data[MAX_BUFSZ];       // 에코할 데이터
} mesg_t;

void child_start(int sock, int pipefd[]);       // 자식 프로세스
void parent_start(int sock, int pipefd[]);      // 부모 프로세스
void errquit(char *mesg) { perror(mesg); exit(1); }//오류 발생시 호출

int main(int argc, char **argv) {
    struct sockaddr_in servaddr;
    pid_t pid;
    int sock, pipefd[2], port, len = sizeof(struct sockaddr);

    if(argc!=2) {
       printf("\n Usage : %s port\n",argv[0]);
       exit(EXIT_FAILURE);
    }

    // 포트번호
    port = atoi(argv[1]);
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0) {
       perror("socket failed");
       exit(EXIT_FAILURE);
    }

    bzero(&servaddr,len);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port  = ntohs(port);
    bind(sock, (struct sockaddr*)&servaddr, len);

    // 파이프 생성
    if(pipe(pipefd) == -1)
      errquit("pipe fail ");

    pid = fork();
    if (pid < 0) errquit("fork fail");
    else if(pid > 0) parent_start(sock, pipefd);
    else if(pid ==0) child_start(sock, pipefd);
    return 0;
}

// 자식 프로세스는 파이프로부터 에코 메시지를 읽고 클라이언트에게 전송
void child_start(int sock,int pipefd[]) {
    mesg_t pmsg;
    int nbytes=0, len = sizeof(struct sockaddr);
    close(pipefd[1]);//쓰기용 디스크립터 제거

    while(1) {
        // 파이프로부터 읽기 대기
        nbytes = read(pipefd[0], (char *)&pmsg, sizeof(mesg_t));
        if(nbytes < 0)
           errquit("read failed ");
        printf("Child  : read from pipe\n");
        // 파이프로부터 읽은 데이터를 에코
        nbytes = sendto(sock, &pmsg.data, strlen(pmsg.data), 0,
            				(struct sockaddr*)&pmsg.addr, len);
        printf("Child  : %d bytes echo response\n", nbytes);
        printf("--------------------------------\n");
    }
}

// 부모 프로세스는 클라이안트로부터 받은메시지를 파이프에 write
void parent_start(int sock, int pipefd[]) {
    mesg_t pmsg;
    int nbytes, len = sizeof(struct sockaddr);

    // 읽기 파이프 닫음
    close( pipefd[0] );

    printf("my echo server wait...\n");
    while(1) {
        // 소켓으로부터 읽기
        nbytes = recvfrom(sock, (void*)&pmsg.data, MAX_BUFSZ, 0, 
					(struct sockaddr*)&pmsg.addr, &len);
        if(nbytes < 0) errquit("recvfrom failed ");
        printf("Parent : %d bytes recv from socket\n", nbytes);
        pmsg.data[nbytes]=0;

        // 소켓으로부터 읽은 데이터를 파이프에 쓰기
        if( write(pipefd[1], (char*)&pmsg, sizeof(pmsg))<0 )
            perror("write fail ");
        printf("Parent : write to pipe\n");
    }
}