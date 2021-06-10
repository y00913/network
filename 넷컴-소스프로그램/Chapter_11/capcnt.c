//--------------------------------------------------------------
// ���ϸ� : capcnt.c
// ��  �� : ��Ŷ�� ĸ���ϰ� ��Ŷ�� ũ�⸦ ���
// ������ : gcc -o capcnt capcnt.c -lpcap
// ��  �� : capcnt
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

// ��Ŷ�� �д� �Լ�
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
    // Ÿ�Ӿƿ��� 3�� ����
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

// ��Ŷ�� �д� �Լ�
void my_packet_read(pcap_t *pd, char ebuf[]) {
    int maxcnt, count;

    // Ÿ�Ӿƿ� �������� callback() ȣ��
    maxcnt = -1;   // ����ؼ� ��Ŷ ĸ��
    if(pcap_loop(pd, maxcnt, callback, (void*)pd) <0 ) {
        fprintf(stderr, "pcap_loop fail: %s\n", ebuf);
        exit(1);
  }
}

// pcap_pkthdr ����ü�� �̿��� ���� ���
void callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    static int count=0;
    count++;
    printf("%d ��° ��Ŷ \n", count);
    printf("\tĸ���� ũ��  =%d byte\n", pkthdr->caplen);
    printf("\t��Ŷ�� ���� ���� =%d byte\n", pkthdr->len);
    return;
}