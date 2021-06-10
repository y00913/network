//--------------------------------------------------------------
// 파일명 : echoserv.c
// 동  작 : Xinetd에 등록되어 에코 요청 서비스를 함
// 컴파일 : gcc -o myecho_serv echoserv.c
// 실  행 : Xinetd에 의해 클라이언트의 요청이 들어오면 자동실행
//--------------------------------------------------------------
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

int main() {
    int cnt = 0;
    char line[1024];

    setvbuf(stdout, NULL, _IOLBF, 0);
    while(fgets(line, sizeof(line), stdin) != NULL)
        printf("%3i : %s", ++cnt, line);
    return 0;
}