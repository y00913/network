//--------------------------------------------------------------
// 파일명 : open_socket.c
// 기  능 : socket() 시스템 콜을 호출하고, 생성된 소켓번호를 출력
// 컴파일 : cc -o open_socket open_socket.c 
// 사용법 : open_socket
//--------------------------------------------------------------
#include <stdio.h>      // 표준 입출력
#include <stdlib.h>     // 표준 라이브러리
#include <sys/types.h>  // 소켓 시스템 콜에 필요한 상수 선언
#include <sys/stat.h>   // 파일의 상태에 대한 데이터 선언
#include <sys/socket.h> // 소켓 시스템 콜 선언
#include <fcntl.h>      // open에 필요한 flag 선언
#include <unistd.h>	// standard symbolic constants and types

int main() {
   // 파일 및 소켓번호
   int fd1, fd2, sd1, sd2;
   // 파일 열기
   fd1 = open("/etc/passwd", O_RDONLY, 0);
   printf("/etc/passwd's file descriptor = %d\n", fd1);
   // 스트림형 소켓 열기
   sd1 = socket(PF_INET, SOCK_STREAM, 0);
   printf("stream socket descriptor = %d\n", sd1);

   // 데이터그램형 소켓 열기
   sd2 = socket(PF_INET, SOCK_DGRAM, 0);
   printf("datagram socket descriptor = %d\n", sd2);
	   // 또 다른 파일 열기
   fd2 = open("/etc/hosts", O_RDONLY, 0);
   printf("/etc/hosts's file descriptor = %d\n", fd2);
   // 파일 및 소켓 닫기
   close(fd2);
   close(fd1);
   close(sd2);
   close(sd1);
   return 0;
}

