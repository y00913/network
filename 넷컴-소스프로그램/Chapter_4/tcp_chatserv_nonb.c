//-------------------------------------------------------------
// ���ϸ� : tcp_chatserv_nonb.c
// ��  �� : �ͺ�� ����� ä�� ����
// ������ : cc -o tcp_chatserv_nonb tcp_chatserv_nonb
// ���� : tcp_chatserv_nonb  4001
//-------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE  511
#define MAX_SOCK 1024 //�ֶ󸮽��� 64

char *EXIT_STRING = "exit";
char *START_STRING = "Connected to chat_server\n";
int maxfdp1;                // �ִ� ���Ϲ�ȣ +1
int num_chat = 0;           // ä�� ������ ��
int clisock_list[ MAX_SOCK ]; // ä�ÿ� ������ ���Ϲ�ȣ ���
int listen_sock;

// ���ο� ä�� ������ ó��
void addClient(int s, struct sockaddr_in *newcliaddr);
void removeClient(int);        // ä�� Ż�� ó�� �Լ�
int set_nonblock(int sockfd);  // ������ �ͺ������ ����
int is_nonblock(int sockfd);   // ������ �ͺ�� ������� Ȯ��
int tcp_listen(int host, int port, int backlog); // ���� ���� �� listen
void   errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    int i, j, nbyte, count;
    int accp_sock, clilen;
    struct sockaddr_in  cliaddr;

    if(argc != 2) {
        printf("���� :%s port\n", argv[0]);
       exit(0);
   }

    listen_sock = tcp_listen(INADDR_ANY,atoi(argv[1]),5);
    if(listen_sock==-1)
        errquit("tcp_listen fail");
    if(set_nonblock(listen_sock) == -1)
        errquit("set_nonblock fail");

    for(count=0; ;count++) {
        if(count==100000) {
           putchar('.' );
           fflush(stdout); count=0;
        }
        addrlen = sizeof(cliaddr);
        accp_sock= accept(listen_sock, (struct sockaddr *) &cliaddr, &clilen);
        if(accp_sock == -1 && errno!=EWOULDBLOCK)
            errquit("accept fail");
        else if(accp_sock >0) {
            // ä�� Ŭ���̾�Ʈ ��Ͽ� �߰�
            //clisock_list[num_chat] = accp_sock;
            // ��ſ� ������ �ͺ�� ��尡 �ƴ�
            if(is_nonblock(accp_sock)!=0 && set_nonblock(accp_sock)<0 )
                errquit("set_nonblock fail");
            addClient(accp_sock,&cliaddr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("%d��° ����� �߰�.\n", num_chat);
        }

        // Ŭ���̾�Ʈ�� ���� �޽����� ��� Ŭ���̾�Ʈ���� ���
        for(i = 0; i < num_chat; i++) {
            errno = 0;
            nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
            if(nbyte==0) { // abrupt exit(reset, ctrl-C)
                removeClient(i);    
                continue;
            }
            else if( nbyte==-1 && errno == EWOULDBLOCK) //���� ������ ����, ��� ����
                continue;
            // ���Ṯ�� ó��
            if(strstr(buf, EXIT_STRING) != NULL) {
                removeClient(i);    // abrupt exit
                continue;
            }
            // ��� ä�� �����ڿ��� �޽��� ���
            buf[nbyte] = 0;
            for (j = 0; j < num_chat; j++)
                send(clisock_list[j], buf, nbyte, 0);
            printf("%s\n", buf);
        }
    }
}

// ���ο� ä�� ������ ó��
void addClient(int s, struct sockaddr_in *newcliaddr) {
    char buf[20];
    inet_ntop(AF_INET,&newcliaddr->sin_addr,buf,sizeof(buf));
    printf("new client: %s\n",buf);
     // ä�� Ŭ���̾�Ʈ ��Ͽ� �߰�
    clisock_list[num_chat] = s;
    num_chat++;
}
    // ä�� Ż�� ó��
void removeClient(int i) {
    close(clisock_list[i]);
    if(i != num_chat-1)
        clisock_list[i] = clisock_list[num_chat-1];
    num_chat--;
    printf("ä�� ������ 1�� Ż��. ���� ������ �� = %d\n", num_chat);
}

// ������ nonblock ���� Ȯ��
int is_nonblock(int sockfd) {
    int val;
    // ������ �÷��� ���� ���´�
    val=fcntl(sockfd, F_GETFL,0);
    // �ͺ�� ������� Ȯ��
    if(val & O_NONBLOCK)
        return 0;
    return -1;
}

    // ������ �ͺ�� ���� ����
int set_nonblock(int sockfd) {
    int val;
    // ������ �÷��� ���� ���´�
    val=fcntl(sockfd, F_GETFL,0);
    if(fcntl(sockfd, F_SETFL, val | O_NONBLOCK) == -1)
        return -1;
    return 0;
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
    if (bind(sd , (struct sockaddr *)&servaddr, sizeof(servaddr))
        < 0) {
        perror("bind fail");  exit(1);
    }
    // Ŭ���̾�Ʈ�κ��� �����û�� ��ٸ�
    listen(sd, backlog);
    return sd;
}