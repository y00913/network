//--------------------------------------------------------------
//���ϸ� : myping.c
//��  �� : ������ ping ���α׷�
//������ : gcc -o myping myping.c
//��  �� : myping ip_addr
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>

#define BUFSIZE 4096

int seqnum;              // ping �޽��� �Ϸù�ȣ
char recvbuf[BUFSIZE];   // ���Ź���
char sendbuf[BUFSIZE];   // �۽Ź���
int rawsock;             // Raw ���� ��ȣ
int notrecv = 0;         // ping ������ ���� ���� ȸ��
struct sockaddr_in sendaddr, recvaddr;

int send_ping();                                    	// ping request
int prn_rcvping(char *ipdata, int recvsize);     	// ping ���信 ���
void prn_icmp(struct icmphdr  *icmp, int icmpsize); 	// ICMP ��� ���
unsigned short in_cksum(unsigned short *addr, int len); // ICMP Check sum
void errquit(char *msg) { perror(msg); exit(0); }

int main(int argc, char **argv) {
    int recvsize,
       addrlen=sizeof(struct sockaddr);
    fd_set readset;
    struct timeval tv;
    int ret;

    if(argc != 2) {
        printf("Usage : %s  ip_address \n",argv[0]);
        exit(1);
    }

    addrlen = sizeof(struct sockaddr);
    bzero(&recvaddr, sizeof(struct sockaddr));
    bzero(&sendaddr, sizeof(struct sockaddr));

    sendaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &sendaddr.sin_addr.s_addr);
    sendaddr.sin_port = htons(0);

    // raw ���� ����
    rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(rawsock<0) errquit("socket fail");

    // Ŀ�ο� ����� �ּҸ� ����ص�, connected UDP
    //if(connect(rawsock, (struct sockaddr*)&sendaddr, sizeof(struct sockaddr) )!=0)
    //    errquit("connect fail ");

    // ù��° ping ������
    while(1) {
        FD_ZERO(&readset);
        FD_SET(rawsock, &readset);
        tv.tv_sec =  1; // 1�� Ÿ�̸�
        tv.tv_usec = 0;
        send_ping();    // ping�� ����
        ret = select(rawsock+1, &readset, NULL,NULL,&tv); // Ÿ�̸� ����
        if(ret == 0) {    // Ÿ�Ӿƿ�
            if(++notrecv == 3) {
                notrecv = 0;
                puts("Request Timeout ... ");
            }
            continue;
        }
        else if(ret <0) // select()�� ����
            errquit("select fail");

        // select()�� ������, ping ������ ����
        recvsize = recvfrom(rawsock, recvbuf, sizeof(recvbuf),0,
                		(struct sockaddr*)&recvaddr, &addrlen);
        if(recvsize<0)
            errquit("recvfrom fail ");
        notrecv = 0; // ���ŵ� ���信 ���� ó��
        prn_rcvping(recvbuf, recvsize);// ping �޽��� ���
        sleep(1);// 1�� �������� ping�� ����
    }
    exit(0);
}

void prn_icmp(struct icmphdr  *icmp, int icmpsize) {
    printf("[icmp](id:%d ", icmp->un.echo.id);
    printf("seq:%d "      , icmp->un.echo.sequence);
    printf("code:%d "     , icmp->code);
    printf("type:%d )\n"  , icmp->type);
}

// ���ŵ� �޽����� ���
int prn_rcvping(char *ipdata, int recvsize) {
    int ip_headlen ,
       icmp_len;
    struct icmphdr *icmp;
    struct iphdr *ip;
    char buf[512];

    ip = (struct iphdr*)ipdata;
    ip_headlen= ip->ihl * 4;		//��� ũ��, ������ 351�� ����
    icmp_len= recvsize - ip_headlen;	// �޽��� ũ��
    icmp = (struct icmphdr *) (ipdata + ip_headlen);// �޽��� ���� �ּ�
    if (icmp->type != ICMP_ECHOREPLY)
        return -1;
    inet_ntop(AF_INET,(void*)&ip->saddr, buf,sizeof(buf));// IP �ּ�-> dotted decimal
    printf("%d bytes recv from (%s) ",icmp_len, buf);
    prn_icmp(icmp, icmp_len);
    return 0;
}

// ping Request ������
int send_ping() {
    struct icmphdr *icmp;
    int len, sendsize;
    icmp = (struct icmphdr *) sendbuf;
    bzero((char *)icmp, sizeof(struct icmp));
    icmp->code = 0 ;
    icmp->type  = ICMP_ECHO; 		// ICMP_ECHO = 8
    icmp->un.echo.sequence = seqnum++;	// Ping �޽��� �Ϸù�ȣ
    icmp->un.echo.id = getpid();    	// pid �� ID�� ����
    icmp->checksum = 0;        		// checksum ����� �ݵ�� zero
    icmp->checksum=in_cksum((unsigned short *)icmp, sizeof(struct icmp));
    len = sizeof(struct icmphdr);  	// 8 byte
    sendsize = sendto(rawsock, sendbuf, len, 0,
                          (struct sockaddr*)&sendaddr, sizeof(struct sockaddr));
    prn_icmp(icmp,sendsize); 		// ICMP ��� ���
    return sendsize;
}

// checksum ���ϱ� : �����͸� 16��Ʈ�� �����ؼ� ��� 16��Ʈ ���� 1�Ǻ��� ���� 1�� �����̴�.
unsigned short in_cksum(unsigned short *addr, int len) {
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;

    while(nleft>1) {
        sum += *w++; nleft -= 2;
    }
    
    if( nleft==1) { //nleft==1
        *(unsigned char *) (&answer) = *(unsigned char *)w;
        sum += answer;
    }
    //���� 16��Ʈ�� ���� 16��Ʈ�� ���ؼ� 1�� ������ ���� ���Ѵ�.
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum>>16); 	//���� 16��Ʈ�� �ø��� ���� ������ �ٽ� �����ش�.
    answer = ~sum;	//������ ���Ѵ�.
    return (answer);
}
