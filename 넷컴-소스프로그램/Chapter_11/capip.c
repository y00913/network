//--------------------------------------------------------------
// ���ϸ� : capip.c
// ��  �� : IP ��Ŷ�� ĸ���Ͽ� ���
// ������ : gcc -o capip capip.c -lpcap
// ��  �� : capip
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
    pcap_t  *pd;                     // pcap  ��ũ����
    char     ebuf[PCAP_ERRBUF_SIZE], // error buffer
            *dev;                    // device name
    int      snapsize = 68;          // Snap Length
    struct in_addr      netmask,     // �ݸ���ũ
                        network;     // ��Ʈ��ũ �ּ�
    struct bpf_program  fcode;       // ��Ŷ���͸� ���α׷�

    dev = pcap_lookupdev(ebuf);   // ����̽� ã��

    // Ÿ�Ӿƿ�  2��
    pd=pcap_open_live(dev, snapsize, 1, 2000, ebuf); // ����̽� ����
    if(pd == NULL) {
        fprintf(stderr, "pcap_open_live fail: %s", ebuf);
        exit(1);
    }

    // ����̽��� �ش��ϴ� localnet�� netmask�� ����
    pcap_lookupnet(dev, &network.s_addr, &netmask.s_addr, ebuf);

    // ���͸� ��Ģ ������
    pcap_compile(pd, &fcode, "ip", 0, netmask.s_addr);
    pcap_setfilter(pd, &fcode); // ��ũ���Ϳ� ���͸� ��Ģ����

    printf("device='%s'(network=%s, netmask=%s)\n",
            		dev, inet_ntoa(network), inet_ntoa(netmask));
    puts("capture start ... ");
    my_packet_read(pd);
    pcap_close(pd);
    exit(0);
}

// ��Ŷ�� �а� callback �Լ��� ȣ��
void my_packet_read(pcap_t *pd) {
    if(pcap_loop(pd, -1, callback, NULL)<0) {
        fprintf(stderr, "pcap_loop fail: %s\n", pcap_geterr(pd));
        exit(1);
    }
}

// ���ŵ� ��Ŷ�� ���� ó��
void callback(  u_char *user,
         const struct pcap_pkthdr *h, const u_char *packet) {
    struct iphdr *iph;
    struct ethhdr *eh = (struct ethhdr *) packet;
    int ethhdr_len = sizeof(struct ethhdr);

    // �̴��� ������ ����� �����κ��� IP �����
    iph = (struct iphdr *) (packet + ethhdr_len);
    prn_iphdr(iph); // IP��� ���
    puts("====================================\n");
    return;
}

// IP ��� ���
void prn_iphdr(struct iphdr *iph) {
    char   buf[20];
    int    fragoff;
    int    DF; // don't fragment

    printf("(ver:%d)",      iph->version);
    printf("(hlen:%d)",     iph->ihl*4);
    printf("(tos:%d)",      iph->tos);
    printf("(tlen:%d)",     ntohs(iph->tot_len));
    printf("(id:%d)",       ntohs(iph->id));
    DF = ntohs(iph->frag_off) >> 13;    	// ���� 3��Ʈ�� ��ȣȭ
    fragoff = ntohs(iph->frag_off) & 0x1FFF; 	// ���� 13��Ʈ�� ��ȿȭ
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