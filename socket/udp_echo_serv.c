#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE    511

int main(int argc, char *argv[]) {
    struct sockaddr_in servaddr, cliaddr;
    int s, nbyte, addrlen = sizeof(struct sockaddr);
    char buf[MAXLINE+1];

   if(argc != 2) {
        printf("usage: %s port\n", argv[0]);
        exit(0);
    }

   if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket fail");
        exit(0);
   }

    bzero((char *)&servaddr,addrlen);  //seraddr을 '\0'으로 초기화
    bzero((char *)&cliaddr, addrlen);  //cliaddr을 '\0'으로 초기화
    //servaddr 세팅
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    //bind() 호출
    if(bind(s, (struct sockaddr *)&servaddr, addrlen) < 0) {
        perror("bind fail");
        exit(0);
    }
   //iterative 에코 서비스 수행
   while(1)
    {
        puts("Server : waiting request.");
        nbyte = recvfrom(s, buf, MAXLINE , 0,
                      (struct sockaddr *)&cliaddr, &addrlen);
        if(nbyte< 0) {
            perror("recvfrom fail");
            exit(1);
        }
        buf[nbyte] = 0;
        printf("%d byte recv: %s\n",nbyte, buf);
        if (sendto(s, buf, nbyte, 0,
                    (struct sockaddr *)&cliaddr, addrlen) < 0) {
            perror("sendto fail");
            exit(1);
      }
      puts("sendto complete");
  }
}
