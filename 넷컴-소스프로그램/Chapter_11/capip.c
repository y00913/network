//--------------------------------------------------------------
// 파일명 : capip.c
// 동  작 : IP 패킷을 캡쳐하여 출력
// 컴파일 : gcc -o capip capip.c -lpcap
// 실  행 : capip
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

void my_packet_read(pcap_t *pd);
void callback(u_char *user, const struct pcap_pkthdr *h, const u_char *p);
void prn_iphdr(struct iphdr *hdr);

int main(int argc,char **argv) {
    pcap_t  *pd;                     // pcap  디스크립터
    char     ebuf[PCAP_ERRBUF_SIZE], // error buffer
            *dev;                    // device name
    int      snapsize = 68;          // Snap Length
    struct in_addr      netmask,     // 넷마스크
                        network;     // 네트워크 주소
    struct bpf_program  fcode;       // 패킷필터링 프로그램

    dev = pcap_lookupdev(ebuf);   // 디바이스 찾기

    // 타임아웃  2초
    pd=pcap_open_live(dev, snapsize, 1, 2000, ebuf); // 디바이스 열기
    if(pd == NULL) {
        fprintf(stderr, "pcap_open_live fail: %s", ebuf);
        exit(1);
    }

    // 디바이스에 해당하는 localnet과 netmask를 얻음
    pcap_lookupnet(dev, &network.s_addr, &netmask.s_addr, ebuf);

    // 필터링 규칙 컴파일
    pcap_compile(pd, &fcode, "ip", 0, netmask.s_addr);
    pcap_setfilter(pd, &fcode); // 디스크립터에 필터링 규칙적용

    printf("device='%s'(network=%s, netmask=%s)\n",
            		dev, inet_ntoa(network), inet_ntoa(netmask));
    puts("capture start ... ");
    my_packet_read(pd);
    pcap_close(pd);
    exit(0);
}

// 패킷을 읽고 callback 함수를 호출
void my_packet_read(pcap_t *pd) {
    if(pcap_loop(pd, -1, callback, NULL)<0) {
        fprintf(stderr, "pcap_loop fail: %s\n", pcap_geterr(pd));
        exit(1);
    }
}

// 수신된 패킷에 대한 처리
void callback(  u_char *user,
         const struct pcap_pkthdr *h, const u_char *packet) {
    struct iphdr *iph;
    struct ethhdr *eh = (struct ethhdr *) packet;
    int ethhdr_len = sizeof(struct ethhdr);

    // 이더넷 프레임 헤더의 다음부분이 IP 헤더임
    iph = (struct iphdr *) (packet + ethhdr_len);
    prn_iphdr(iph); // IP헤더 출력
    puts("====================================\n");
    return;
}

// IP 헤더 출력
void prn_iphdr(struct iphdr *iph) {
    char   buf[20];
    int    fragoff;
    int    DF; // don't fragment

    printf("(ver:%d)",      iph->version);
    printf("(hlen:%d)",     iph->ihl*4);
    printf("(tos:%d)",      iph->tos);
    printf("(tlen:%d)",     ntohs(iph->tot_len));
    printf("(id:%d)",       ntohs(iph->id));
    DF = ntohs(iph->frag_off) >> 13;    	// 앞의 3비트만 유호화
    fragoff = ntohs(iph->frag_off) & 0x1FFF; 	// 뒤의 13비트만 유효화
    if( DF == 1) printf("(More Fragment, offset:%d)", DF/*,fragoff*/);
    if( DF == 2) printf("(Don't Fragment)", DF);
    printf("(ttl:%d)\n",    iph->ttl);
    printf("(proto:0x%x)",  iph->protocol);
    printf("(cksum:0x%x)",  ntohs(iph->check));
    if( inet_ntop(AF_INET, &iph->saddr,buf,sizeof(buf)) == -1) {
        perror("inet_ntop fail");
        exit(1);
    }
    printf("(saddr:%s)",    buf);
    if( inet_ntop(AF_INET, &iph->daddr,buf,sizeof(buf)) == -1) {
        perror("inet_ntop fail");
        exit(1);
    }
    printf("(daddr:%s)\n",  buf);
    fflush(stdout);
}