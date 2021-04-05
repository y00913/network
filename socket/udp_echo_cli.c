#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE  511

int main(int argc, char *argv[]) {
    struct sockaddr_in servaddr;
    int s, nbyte, addrlen = sizeof(servaddr);
    char buf[MAXLINE+1];

    if(argc != 3) {
        printf("usage: %s ip_address port_number\n", argv[0]);
        exit(0);
    }

    if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket fail");
       exit(0);
    }

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    //키보드 입력
    printf("send : ");
    if (fgets(buf, MAXLINE , stdin)==NULL) {
        printf("fgets fail");
        exit(0);
    }
   //에코 서버로 메시지 송신
   if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, addrlen) < 0) {
        perror("sendto fail");
        exit(0);
    }
   //수신된 에코 메시지
    if ( (nbyte=recvfrom(s, buf, MAXLINE, 0,
                (struct sockaddr *)&servaddr, &addrlen)) < 0) {
        perror("recevfrom fail");
        exit(0);
    }
    buf[ nbyte ]=0;  //메시지의 끝에 '\0' 추가
    printf("%s\n", buf);
    close(s);
    return 0;
}
