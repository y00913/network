//tcp_talk_serv 8080

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char *EXIT_STRING = "exit"; //종료 문자
int recv_and_print(int sd); //상대로부터 메시지 수신후 화면 출력, 자식 프로세스에서 수행
int input_and_send(int sd); //키보드 입력받고 상대에게 메시지 전달, 부모 프로세스에서 수행

#define MAXLINE 511

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr, servaddr;
    int listen_sock, accp_sock, addrlen = sizeof(cliaddr); //소켓 번호, 주소 길이
    pid_t pid;
    if (argc != 2) {
        printf("사용법: %s port\n", argv[0]);
        exit(0);
    }
    //소켓 생성
    if((listen_sock=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(0);
    }
    //서버의 소켓 주소 구조체 servaddr을 '0'으로 초기화
    bzero((char *)&servaddr, sizeof(servaddr));
    // servaddr을 세팅
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    // bind() 호출
    if(bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");
        exit(0);
    }
    puts("서버가 클라이언트를 기다리고 있습니다");
    listen(listen_sock, 1);
    //클라이언트 연결 요청 수락
    if((accp_sock = accept(listen_sock,(struct sockaddr *)&cliaddr, &addrlen)) < 0) {
        perror("accept fail");
        exit(0);
    }
    puts("클라이언트가 연결되었습니다.");
    if( (pid = fork()) > 0)
        input_and_send(accp_sock); //키보드 입력 받고 상대에게 메시지 전달
    else if (pid == 0)
        recv_and_print(accp_sock); //상대로부터 메시지 수신후 화면 출력
    close(listen_sock);
    close(accp_sock);
    return 0;
}

int input_and_send(int sd) {
    char buf[MAXLINE+1];
    int nbyte;
    while(fgets(buf, sizeof(buf), stdin) != NULL) {
        nbyte = strlen(buf);
        write(sd, buf, strlen(buf));
        //종료 문자열 수신시 종료
        if (strstr(buf, EXIT_STRING) != NULL ) {
            puts("Good bye.");
            close(sd);
            exit(0);
        }
    }
    return 0;
}

int recv_and_print(int sd) {
    char buf[MAXLINE+1];
    int nbyte;
    while(1) {
        if( (nbyte=read(sd, buf, MAXLINE))<0){
            perror("read fail");
            close(sd);
            exit(0);
        }
        buf[nbyte] = 0;
        //종료 문자열 수신시 종료
        if (strstr(buf, EXIT_STRING) != NULL )
            break;
        printf("%s", buf); //화면 출력
    }
    return 0;
}
