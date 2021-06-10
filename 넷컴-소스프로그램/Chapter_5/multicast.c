//--------------------------------------------------------------
// ���ϸ� : multicast.c
// ��  �� : ��Ƽĳ��Ʈ�� �̿��� ä�� ���α׷�
// ������ : cc -o multicast multicast.c
// ���� : multicast 239.0.3.3 3000 My_name
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 1023

int main(int argc, char *argv[]) {
    int send_s, recv_s,         // ���ſ� �� �۽ſ� ����
       n, len;
    pid_t pid;
    unsigned int yes = 1;
    struct sockaddr_in mcast_group;// ��Ƽĳ��Ʈ �׷� �ּ�
    struct ip_mreq mreq;
    char name[10];                  // ä�� ������ �̸�

    if (argc != 4) {
        printf("����: %s multicast_address port My_name \n", argv[0]);
        exit(0);
    }
    sprintf(name, "[%s]", argv[3]);

    memset(&mcast_group, 0, sizeof(mcast_group));
    mcast_group.sin_family = AF_INET;
    mcast_group.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&mcast_group.sin_addr);

    // ��Ƽĳ��Ʈ ���ſ� ���� ����
    if ( (recv_s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("error: Can't create receive socket\n");
        exit(0);
    }

    // ��Ƽĳ��Ʈ �׷쿡 ����
    mreq.imr_multiaddr = mcast_group.sin_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(recv_s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))<0) {
        printf("error: add membership\n");
        exit(0);
    }

    // ���� ���� �ɼ� ����
    if (setsockopt(recv_s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        printf("error : reuse setsockopt\n");
        exit(0);
    }
    // ���� ���ε�
    if (bind(recv_s, (struct sockaddr*)&mcast_group, sizeof(mcast_group)) < 0) {
        printf("error : bind receive socket\n");
        exit(0);
    }

    // ��Ƽĳ��Ʈ �޽��� �۽ſ� ���� ����
    if ((send_s=socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("error : Can't create send socket\n");
        exit(0);
    }

    // fork() ����: child�� ���� ���, parent�� �۽� ���
    if ((pid=fork()) < 0) {
        printf("error : fork\n");
        exit(0);
    }

    // child process : ä�� �޽��� ���� ���
    else if (pid == 0) {
        struct sockaddr_in from;
        char message[MAXLINE+1];
        for (;;) {
            printf("receiving message...\n");
            len = sizeof(from);
            if ((n=recvfrom(recv_s, message, MAXLINE,0,
				(struct sockaddr*)&from, &len))<0) {
                printf("error : recvfrom\n");
                exit(0);
            }
            message[n] = 0;
            printf("Receiced Message: %s\n", message);
        }
    }

    // parent process : Ű���� �Է� �� �޽��� �۽� ���
    else {
        char message [MAXLINE+1], line[MAXLINE+1];
        printf("Send Message :");
        while (fgets(message, MAXLINE, stdin) != NULL) {
            sprintf(line, "%s %s", name, message);
            len = strlen(line);
            if(sendto(send_s, line, strlen(line), 0,
                (struct sockaddr*)&mcast_group, sizeof(mcast_group)) < len) {
                printf("error : sendto\n");
                exit(0);
            }
        }
    }
    return 0;
}