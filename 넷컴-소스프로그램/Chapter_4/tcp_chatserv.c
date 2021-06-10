//-------------------------------------------------------------
// ���ϸ� : tcp_chatserv.c
// ��  �� : ä�� ������ ����, ä�� �޽��� ���� �� ���
// ������ : cc -o chat_server tcp_chatserv.c
// ���� : chat_server 4001
//-------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE  511
#define MAX_SOCK 1024 // �ֶ󸮽��� ��� 64

char *EXIT_STRING = "exit";   // Ŭ���̾�Ʈ�� �����û ���ڿ�
// Ŭ���̾�Ʈ ȯ�� �޽���
char *START_STRING = "Connected to chat_server \n";
int maxfdp1;                // �ִ� ���Ϲ�ȣ +1
int num_chat = 0;          // ä�� ������ ��
int clisock_list[MAX_SOCK]; // ä�ÿ� ������ ���Ϲ�ȣ ���
int listen_sock;             // ������ ���� ����

void addClient(int s, struct sockaddr_in *newcliaddr);// ���ο� ä�� ������ ó��
int getmax();               // �ִ� ���� ��ȣ ã��
void removeClient(int s);    // ä�� Ż�� ó�� �Լ�
int tcp_listen(int host, int port, int backlog); // ���� ����, bind, listen
void errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
    struct sockaddr_in cliaddr;
    char buf[MAXLINE+1];
    int i, j, nbyte, accp_sock, addrlen = sizeof(struct sockaddr_in);
    fd_set read_fds;  // �б⸦ ������ fd_set ����ü

    if(argc != 2) {
        printf("���� :%s port\n", argv[0]);
        exit(0);
    }

    // tcp_listen(host, port, backlog) �Լ� ȣ��
    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
    while(1) {
        FD_ZERO(&read_fds);
        FD_SET(listen_sock, &read_fds);
        for(i=0; i<num_chat; i++)
            FD_SET(clisock_list[i], &read_fds);
        maxfdp1 = getmax() + 1;     // maxfdp1 �� ���
        puts("wait for client");
        if(select(maxfdp1, &read_fds,NULL,NULL,NULL)<0)
            errquit("select fail");

        if(FD_ISSET(listen_sock, &read_fds)) {//new client
           accp_sock=accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
            if(accp_sock == -1)
                errquit("accept fail");
            addClient(accp_sock, &cliaddr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("%d��° ����� �߰�.\n", num_chat);
        }

        // Ŭ���̾�Ʈ�� ���� �޽����� ��� Ŭ���̾�Ʈ���� ���
        for(i = 0; i < num_chat; i++) {
           if(FD_ISSET(clisock_list[i], &read_fds)) {
                nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
                if(nbyte<= 0) { // ������������ ����(ctrl-C)
                    removeClient(i);    
                    continue;
                }
                buf[nbyte] = 0;
                // ���Ṯ�� ó��
                if(strstr(buf, EXIT_STRING) != NULL) {
                    removeClient(i);    // Ŭ���̾�Ʈ�� ����
                    continue;
                }
                // ��� ä�� �����ڿ��� �޽��� ���
                for (j = 0; j < num_chat; j++)
                    send(clisock_list[j], buf, nbyte, 0);
                printf("%s\n", buf);
            }
        }
    }  // end of while
    return 0;
}

// ���ο� ä�� ������ ó��
void addClient(int s, struct sockaddr_in *newcliaddr) {
    char buf[20];
    // Ŭ���̾�Ʈ �ּҸ� dotted decimal�� ��ȯ
    inet_ntop(AF_INET,&newcliaddr->sin_addr,buf,sizeof(buf));
    printf("new client: %s\n",buf);
     // ä�� Ŭ���̾�Ʈ ��Ͽ� �߰�
    clisock_list[num_chat] = s;
    num_chat++;
}

// ä�� Ż�� ó��
void removeClient(int s) {
    close(clisock_list[s]);
    if(s != num_chat-1)
        clisock_list[s] = clisock_list[num_chat-1];
    num_chat--;
    printf("ä�� ������ 1�� Ż��. ���� ������ �� = %d\n", num_chat);
}

// �ִ� ���Ϲ�ȣ ã��
int getmax() {
    // Minimum ���Ϲ�ȣ�� ���� ���� ������ listen_sock
    int max = listen_sock;
    int i;
    for (i=0; i < num_chat; i++)
        if (clisock_list[i] > max )
            max = clisock_list[i];
    return max;
}

// listen ���� ���� �� listen
int  tcp_listen(int host, int port, int backlog) {
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1) {
        perror("socket fail");
        exit(1);
    }
    // servaddr ����ü�� ���� ����
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);
    if (bind(sd , (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");  exit(1);
    }
    // Ŭ���̾�Ʈ�κ��� �����û�� ��ٸ�
    listen(sd, backlog);
    return sd;
}
