//--------------------------------------------------------------
// ���ϸ� : tcp_talkcli.c
// ��  �� : ��ũ ������ 1:1 ����� �ϴ� Ŭ���̾�Ʈ ���α׷�
// ������ : cc -o talk_client tcp_talkcli.c
// ���� : talk_client 192.203.144.11 3000
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE   511

char *EXIT_STRING = "exit"; // ���Ṯ�� ����
int recv_and_print(int sd); // ���κ��� �޽��� ������ ȭ�����
int input_and_send(int sd); // Ű�����Է¹ް� ��뿡�� �޽�������

int main(int argc, char *argv[]) {
   pid_t pid;
   static int s;
   static struct sockaddr_in servaddr;

   // ��ɹ� �Է� ���� ó��
   if (argc != 3) {
      printf("����: %s server_ip port \n", argv[0]);
      exit(0);
   }
   // ���� ����
   if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      printf("Client: Can't open stream socket.\n");
      exit(0);
   }
   // servaddr�� '0'���� �ʱ�ȭ
   bzero((char *)&servaddr, sizeof(servaddr));
   // servaddr ����
   servaddr.sin_family = AF_INET;
   inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
   servaddr.sin_port = htons(atoi(argv[2]));
   // ������ �����û
   if(connect(s,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
      printf("Client: can't connect to server.\n");
      exit(0);
   }
   if( (pid=fork())>0) // �θ� ���μ���
       input_and_send(s);
   else if(pid==0)   // �ڽ� ���μ���
       recv_and_print(s);
    close(s);
    return 0;
}

// Ű�����Է¹ް� ��뿡�� �޽�������
int input_and_send(int sd) {
    char buf[MAXLINE+1];
    int nbyte;
    while(fgets(buf, sizeof(buf), stdin) != NULL) {
        nbyte = strlen(buf);
        write(sd, buf, strlen(buf));
        // ���Ṯ�ڿ� �Է� ó��
        if (strstr(buf, EXIT_STRING) != NULL ) {
            puts("Good bye.");
            close(sd);
            exit(0);
        }
    }
    return 0;
}

// ���κ��� �޽��� ������ ȭ�����
int recv_and_print(int sd) {
    char buf[MAXLINE+1];
    int nbyte;
    while(1) {
        if( (nbyte=read(sd, buf, MAXLINE))<0) {
            perror("read fail");
            close(sd);
            exit(0);
        }
        buf[nbyte] = 0;
        // ���Ṯ�ڿ� ���Ž� ����
        if (strstr(buf, EXIT_STRING) != NULL )
            break;
        printf("%s", buf); // ȭ�� ���
    }
    return 0;
}