//--------------------------------------------------------------
// 파일명 : captcp.c
// 동  작 : TCP 패킷을 캡쳐하여 출력
// 컴파일 : gcc -o captcp captcp.c -lpcap
// 실  행 : captcp
//--------------------------------------------------------------
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <pcap.h>

void my_packet_read(pcap_t *pd) ;
void callback(  u_char *user, const struct pcap_pkthdr *h, const u_char *p);
void prn_tcphdr(struct tcphdr *tcph);

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
    pd=pcap_open_live(dev,snapsize,1,2000,ebuf); // 디바이스 열기
    if(pd == NULL) {
        fprintf(stderr, "pcap_open_live fail: %s", ebuf);
        exit(1);
    }

    // 디바이스에 해당하는 localnet과 netmask를 얻음
    pcap_lookupnet(dev, &network.s_addr, &netmask.s_addr, ebuf);

    // 필터링 규칙 컴파일
    pcap_compile(pd, &fcode, "tcp", 0, netmask.s_addr);
    pcap_setfilter(pd, &fcode); // 디스크립터에 필터링 규칙적용

    printf("device='%s'(network=%s, netmask=%s)\n",
		            dev, inet_ntoa(network), inet_ntoa(netmask));
    puts("capture start ... ");
    my_packet_read(pd);
    pcap_close(pd);
    exit(0);
}

// 패킷을 읽고 callback 함수를 부름
void my_packet_read(pcap_t *pd) {
    if(pcap_loop(pd, -1, callback, NULL)<0) {
        fprintf(stderr, "pcap_loop fail: %s\n", pcap_geterr(pd));
        exit(1);
    }
}

// 수신된 패킷에 대한 처리
void callback(  u_char *user,
    const struct pcap_pkthdr *h, const u_char *packet) {
    struct tcphdr *tcph;
    struct ip  *iph;
    int    iphlen;
    iph =  (struct ip *) (packet + sizeof(struct ethhdr));
    // IP 헤더의 길이
    iphlen = iph->ip_hl*4;

    // 이더넷 프레임 헤더+ IP 헤더의 다음이 TCP 헤더임
    tcph = (struct tcphdr *) (packet +
            sizeof(struct ethhdr) + iphlen) ;
    prn_tcphdr(tcph); // IP헤더 출력
    puts("====================================\n");
    return;
}

// TCP 헤더 출력
void prn_tcphdr(struct tcphdr *tcph) {
    printf("(th_sport:%d) " , ntohs(tcph->source));
    printf("(th_dport:%d) " , ntohs(tcph->dest));
    printf("(th_seq:%lu)"   , (unsigned int) ntohl(tcph->seq));
    printf("(th_ack:%lu)"   , (unsigned int) ntohl(tcph->ack_seq));
    // TCP Header Length = tcph->doff * 4
    printf("(th_off:%d)\n"  , tcph->doff);
    printf("{th_flags [ ");
    if(tcph->fin)  printf("FIN ");
    if(tcph->syn)  printf("SYN ");
    if(tcph->rst)  printf("RST ");
    if(tcph->psh)  printf("PUSH ");
    if(tcph->ack)  printf("ACK ");
    if(tcph->urg)  printf("URG ");
    printf("]}");

    printf("(th_win:%d)",   ntohs(tcph->window));
    printf("(th_sum:0x%X)", ntohs(tcph->check));
    printf("(th_urp:%d)\n", ntohs(tcph->urg_ptr));
    fflush(stdout);
}