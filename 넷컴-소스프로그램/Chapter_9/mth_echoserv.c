//--------------------------------------------------------------
// 파일명 : mth_echoserv.c
// 동  작 : 멀티스레드 에코 서버
// 컴파일 : gcc -o mth_echoserv  mth_echoserv.c -lpthread
// 실  행 : mth_echoserv  msqkey  port
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_BUFSZ   1024

typedef struct _msg {
    long msg_type;
    struct sockaddr_in addr;
    char msg_text[ MAX_BUFSZ ];
} msg_t ;
struct sockaddr_in servaddr;
int sock;      // 서버 소켓
int msqid;     // 메시지큐 ID

void *echo_recv(void *arg);   // 에코 수신 스레드
void *echo_resp(void *arg);   // 에코 송신 스레드
void errquit(char *msg) { perror(msg); exit(-1);}

void thr_errquit(char *msg,int errcode)
{ printf("%s:%s\n",msg,strerror(errcode)); pthread_exit(NULL);}

int main(int argc, char *argv[]) {
    pthread_t tid[6];
    struct sockaddr_in cliaddr;
    int port, status, i, len = sizeof(struct sockaddr);
    key_t msqkey;

    if(argc!=3) {
        printf("Usage: %s  msgkey port\n", argv[0]);
        exit(1);
    }
    msqkey = atoi(argv[1]);
    port = atoi(argv[2]);

    // 메시지큐 생성
    if( (msqid = msgget(msqkey, IPC_CREAT| 0660))<0)
        errquit("msgget fail");

    // 소켓 생성
    if( (sock = socket(AF_INET, SOCK_DGRAM, 0))<0)
        errquit("socket fail ");

    bzero(&servaddr, len);
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family=AF_INET;
    bind(sock, (struct sockaddr*)&servaddr, len);

    // 수신 스레드 생성
    for(i=0; i<5; i++)
        if((status=pthread_create(&tid[i], NULL, echo_recv, NULL))!=0)
            thr_errquit("pthread_create", status);

	// 송신 스레드 생성
    if((status=pthread_create(&tid[5], NULL, echo_resp, NULL))!=0)
        thr_errquit("pthread_create", status);
	
	//자식 스레드의 종료를 기다림
    for(i=0; i<6; i++)
        pthread_join(tid[i], NULL);

    // 메시지큐 삭제
    msgctl(msqid, IPC_RMID, 0);
    return 0;
}

// 에코 수신 스레드
void *echo_recv(void *arg) {
    int nbytes, status, len = sizeof(struct sockaddr);
    msg_t pmsg;
    int size;
    size = sizeof(pmsg) - sizeof(long);
    pmsg.msg_type = pthread_self();
    while(1) {
        // 에코 메시지 수신
        nbytes = recvfrom(sock, pmsg.msg_text, MAX_BUFSZ, 0,
                            (struct sockaddr *)&pmsg.addr, &len);
        if(nbytes<0)
            thr_errquit("recvfrom fail", errno);
        pmsg.msg_text[nbytes] = 0;
        printf("recv thread = %ld\n", pthread_self());
        // 메시지큐로 전송
        if(msgsnd(msqid, &pmsg, size, 0)==-1)
            thr_errquit("msgsnd fail", errno);
    }
}

// 에코 송신 스레드
void *echo_resp(void *arg) {
    msg_t pmsg;
    int nbytes, len = sizeof(struct sockaddr);
    int size;
    size = sizeof(pmsg) - sizeof(long);
    pmsg.msg_type = 0;// 메시지 큐에 입력된 순서로 메시지 읽음
    while(1) {
        // 메시지큐에서 읽음
        if(msgrcv( msqid,(void *)&pmsg, size, 0, 0) < 0) {
            perror("msgrcv fail"); exit(0);
        }
        // 에코 응답
        nbytes = sendto(sock, pmsg.msg_text, strlen(pmsg.msg_text), 0,
					(struct sockaddr*)&pmsg.addr, len);
        if(nbytes < 0)
            thr_errquit("send to fail",errno);

        printf("response thread = %ld\n\n", pthread_self());
        pmsg.msg_text[0] = 0;
    }
}