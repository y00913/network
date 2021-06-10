//--------------------------------------------------------------
// 파일명 : capcnt.c
// 동  작 : 패킷을 캡쳐하고 패킷의 크기를 출력
// 컴파일 : gcc -o capcnt capcnt.c -lpcap
// 실  행 : capcnt
//--------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <pcap.h>

// 패킷을 읽는 함수
void my_packet_read(pcap_t *pd, char ebuf[]);
void callback( u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet);

int main(int argc,char **argv) {
    char ebuf[ PCAP_ERRBUF_SIZE ], *dev;
    pcap_t *pd;
    int snapsize, promode;
    ebuf[0] = 0;
    dev = pcap_lookupdev(ebuf);
    if(dev == NULL) {
        fprintf(stderr,"pcal_lookupdev fail :%s ",ebuf);
        exit(1);
    }
    else if(ebuf[0] != 0)
        fprintf(stderr,"pcal_lookupdev warning :%s ",ebuf);

    promode = 1;            // promiscuous mode ON
    snapsize = 68;          // snapsize
    // 타임아웃은 3초 간격
    pd = pcap_open_live(dev, snapsize, promode, 3000, ebuf);
    if(pd==NULL) {
        fprintf(stderr, "pcap_open_live fail: %s", ebuf);
        exit(1);
    }

    printf("capture start on %s\n",dev);
    my_packet_read(pd, ebuf);
    pcap_close(pd);
    exit(0);
}

// 패킷을 읽는 함수
void my_packet_read(pcap_t *pd, char ebuf[]) {
    int maxcnt, count;

    // 타임아웃 간격으로 callback() 호출
    maxcnt = -1;   // 계속해서 패킷 캡쳐
    if(pcap_loop(pd, maxcnt, callback, (void*)pd) <0 ) {
        fprintf(stderr, "pcap_loop fail: %s\n", ebuf);
        exit(1);
  }
}

// pcap_pkthdr 구조체를 이용해 정보 출력
void callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    static int count=0;
    count++;
    printf("%d 번째 패킷 \n", count);
    printf("\t캡쳐한 크기  =%d byte\n", pkthdr->caplen);
    printf("\t패킷의 원래 길이 =%d byte\n", pkthdr->len);
    return;
}