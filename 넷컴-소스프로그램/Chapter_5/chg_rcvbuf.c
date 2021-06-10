//--------------------------------------------------------------
// 파일명 : chg_rcvbuf.c
// 동  작 : 소켓 옵션을 사용하여 수신버퍼의 크기 변경
// 컴파일 : gcc -o chg_rcvbuf  chg_rcvbuf.c
// 실  행 : chg_rcvbuf
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int s;
    int val, len;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(1);
    }

    len = sizeof(val);
    if (getsockopt(s, SOL_SOCKET, SO_RCVBUF, &val, &len) < 0) {
        perror("socket fail");
        exit(1);
    }
    printf("디폴트 수신버퍼 크기: %d\n", val);
    val = 1024;
    setsockopt(s, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val));
    getsockopt(s, SOL_SOCKET, SO_RCVBUF, &val, &len);
    printf("1024로 변경한 수신버퍼 크기: %d\n", val);
    return 0;
}