 1	//--------------------------------------------------------------
 2	// 파일명 : ascii_ip.c
 3	// 기  능 : ASCII (dotted decimal)로 표현된 주소를 4 바이트 IP 주소로 변환
 4	// 컴파일 : cc -o ascii_ip ascii_ip.c
 5	// 사용법 : ascii_ip 192.203.144.11
 6	//--------------------------------------------------------------
 7	#include <stdio.h>
 8	#include <stdlib.h>
 9	#include <sys/types.h>
10	#include <sys/socket.h>
11	#include <netinet/in.h>
12	#include <arpa/inet.h>
13	#include <netdb.h>
14
15	int main(int argc, char *argv[]) {
16	    struct in_addr  inaddr;         // 32비트 IP 주소 구조체
17	    char   buf[20];
18
19	    if(argc < 2) {
20	        printf("사용법 : %s IP 주소(dotted decimal) \n", argv[0]);
21	        exit(0);
22	    }
23	    printf("* 입력한 dotted decimal IP 주소: %s\n", argv[1]);
24
25	    inet_pton(AF_INET, argv[1], &inaddr.s_addr);
26	    printf(" inet_pton(%s) = 0x%X\n", argv[1], inaddr.s_addr);
27	    inet_ntop(AF_INET, &inaddr.s_addr, buf, sizeof(buf));
28	    printf(" inet_ntop(0x%X)  = %s\n", inaddr.s_addr,buf);
29
30	    return 0;
31	}

