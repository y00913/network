#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 127

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr, cliaddr;
    int listen_sock, accp_sock,  //소켓 번호
        addrlen = sizeof(cliaddr),  //주소구조체 길이
        nbyte;
    char buf[MAXLINE + 1];  //수신 버퍼

    if (argc != 2)
    {
        printf("usage: %s port\n", argv[0]);
        exit(0);
    }

    if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket fail");
        exit(0);
    }

    bzero((char *)&servaddr, sizeof(servaddr));  // servaddr을 '\0'으로 초기화

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if (bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind fail");
        exit(0);
    }

    //소켓을 수동 대기모드로 세팅
    listen(listen_sock, 5);

    //iterative 에코 서비스 수행
    while (1)
    {
        puts("waiting..");

	//연결요청을 기다림
        accp_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
        if (accp_sock < 0)
        {
            perror("accept fail");
            exit(0);
        }
        puts("connect client..");
        nbyte = read(accp_sock, buf, MAXLINE);
        write(accp_sock, buf, nbyte);
        close(accp_sock);
    }
    close(listen_sock);
    return 0;
}
