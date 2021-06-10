//--------------------------------------------------------------
// ���ϸ� : open_socket.c
// ��  �� : socket() �ý��� ���� ȣ���ϰ�, ������ ���Ϲ�ȣ�� ���
// ������ : cc -o open_socket open_socket.c 
// ���� : open_socket
//--------------------------------------------------------------
#include <stdio.h>      // ǥ�� �����
#include <stdlib.h>     // ǥ�� ���̺귯��
#include <sys/types.h>  // ���� �ý��� �ݿ� �ʿ��� ��� ����
#include <sys/stat.h>   // ������ ���¿� ���� ������ ����
#include <sys/socket.h> // ���� �ý��� �� ����
#include <fcntl.h>      // open�� �ʿ��� flag ����
#include <unistd.h>	// standard symbolic constants and types

int main() {
   // ���� �� ���Ϲ�ȣ
   int fd1, fd2, sd1, sd2;
   // ���� ����
   fd1 = open("/etc/passwd", O_RDONLY, 0);
   printf("/etc/passwd's file descriptor = %d\n", fd1);
   // ��Ʈ���� ���� ����
   sd1 = socket(PF_INET, SOCK_STREAM, 0);
   printf("stream socket descriptor = %d\n", sd1);

   // �����ͱ׷��� ���� ����
   sd2 = socket(PF_INET, SOCK_DGRAM, 0);
   printf("datagram socket descriptor = %d\n", sd2);
	   // �� �ٸ� ���� ����
   fd2 = open("/etc/hosts", O_RDONLY, 0);
   printf("/etc/hosts's file descriptor = %d\n", fd2);
   // ���� �� ���� �ݱ�
   close(fd2);
   close(fd1);
   close(sd2);
   close(sd1);
   return 0;
}

