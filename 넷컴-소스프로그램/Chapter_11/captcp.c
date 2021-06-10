//--------------------------------------------------------------
// ���ϸ� : captcp.c
// ��  �� : TCP ��Ŷ�� ĸ���Ͽ� ���
// ������ : gcc -o captcp captcp.c -lpcap
// ��  �� : captcp
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
    pcap_t  *pd;                     // pcap  ��ũ����
    char     ebuf[PCAP_ERRBUF_SIZE], // error buffer
            *dev;                    // device name
    int      snapsize = 68;          // Snap Length
    struct in_addr      netmask,     // �ݸ���ũ
                        network;     // ��Ʈ��ũ �ּ�
    struct bpf_program  fcode;       // ��Ŷ���͸� ���α׷�

    dev = pcap_lookupdev(ebuf);   // ����̽� ã��

    // Ÿ�Ӿƿ�  2��
    pd=pcap_open_live(dev,snapsize,1,2000,ebuf); // ����̽� ����
    if(pd == NULL) {
        fprintf(stderr, "pcap_open_live fail: %s", ebuf);
        exit(1);
    }

    // ����̽��� �ش��ϴ� localnet�� netmask�� ����
    pcap_lookupnet(dev, &network.s_addr, &netmask.s_addr, ebuf);

    // ���͸� ��Ģ ������
    pcap_compile(pd, &fcode, "tcp", 0, netmask.s_addr);
    pcap_setfilter(pd, &fcode); // ��ũ���Ϳ� ���͸� ��Ģ����

    printf("device='%s'(network=%s, netmask=%s)\n",
		            dev, inet_ntoa(network), inet_ntoa(netmask));
    puts("capture start ... ");
    my_packet_read(pd);
    pcap_close(pd);
    exit(0);
}

// ��Ŷ�� �а� callback �Լ��� �θ�
void my_packet_read(pcap_t *pd) {
    if(pcap_loop(pd, -1, callback, NULL)<0) {
        fprintf(stderr, "pcap_loop fail: %s\n", pcap_geterr(pd));
        exit(1);
    }
}

// ���ŵ� ��Ŷ�� ���� ó��
void callback(  u_char *user,
    const struct pcap_pkthdr *h, const u_char *packet) {
    struct tcphdr *tcph;
    struct ip  *iph;
    int    iphlen;
    iph =  (struct ip *) (packet + sizeof(struct ethhdr));
    // IP ����� ����
    iphlen = iph->ip_hl*4;

    // �̴��� ������ ���+ IP ����� ������ TCP �����
    tcph = (struct tcphdr *) (packet +
            sizeof(struct ethhdr) + iphlen) ;
    prn_tcphdr(tcph); // IP��� ���
    puts("====================================\n");
    return;
}

// TCP ��� ���
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