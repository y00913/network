// port 8080 : echo 서비스
// port 8081 : daytime 서비스

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
#include <time.h>

#define MAXLINE  127

int tcp_listen(int host, int port, int backlog);

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr, servaddr;
    char buf[MAXLINE+1];
    int nbyte, accp_sock, addrlen;
    int listen_sock1, listen_sock2;
    fd_set read_fds;

    //echo 소켓 생성
    listen_sock1 = socket(PF_INET, SOCK_STREAM, 0);
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);
    bind(listen_sock1, (struct sockaddr *)&servaddr, sizeof(servaddr)); 

    //daytime 소켓 생성
    listen_sock2 = socket(PF_INET, SOCK_STREAM, 0);
    servaddr.sin_port = htons(8081);
    bind(listen_sock2, (struct sockaddr *)&servaddr, sizeof(servaddr)); 

    listen(listen_sock1, 5);
    listen(listen_sock2, 5);

    while( 1 ) {
        FD_ZERO(&read_fds);
        FD_SET(listen_sock1, &read_fds);
        FD_SET(listen_sock2, &read_fds);

        puts("wait for client");
        if(select(listen_sock2+1, &read_fds,NULL,NULL,NULL)<0)
            perror("select fail");

        //echo 서비스
        if(FD_ISSET(listen_sock1, &read_fds)) {
            addrlen = sizeof(cliaddr);
            bzero((char *)&cliaddr, addrlen);
            accp_sock=accept(listen_sock1, (struct sockaddr *)&cliaddr, &addrlen);
            if(accp_sock == -1)
                perror("accept fail");
            puts("connet echo..");     
            nbyte = read(accp_sock, buf, MAXLINE);
            write(accp_sock, buf, nbyte);
            close(accp_sock);
        }

        //daytime 서비스
        if(FD_ISSET(listen_sock2, &read_fds)) {
            addrlen = sizeof(cliaddr);
            accp_sock=accept(listen_sock2, (struct sockaddr *)&cliaddr, &addrlen);
            if(accp_sock == -1)
                perror("accept fail");
            puts("connet daytime..");

            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            strftime( buf, sizeof(buf), "%c\n", t);
            nbyte = sizeof(buf);
            write(accp_sock, buf, nbyte);
            close(accp_sock);
        }
    } 
    close(listen_sock1);
    close(listen_sock2);
    return 0;
}
