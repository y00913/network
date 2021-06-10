//--------------------------------------------------------------
// ���ϸ� : byte_order.c
// ��  �� : ȣ��Ʈ ����Ʈ ���� �׽�Ʈ ���α׷�
// ������ : cc -o byte_order byte_order.c -lsocket
// ���� : byte_order
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
int main(void) {
  struct servent *servent;
  servent = getservbyname("echo", "udp");
  if( servent == NULL )  {
    printf( "���� ������ ���� �� ����. \n\n");
    exit(0);
  }

  printf("UDP ���� ��Ʈ��ȣ(��Ʈ��ũ ����) : %d \n", servent->s_port);
  printf("UDP ���� ��Ʈ��ȣ(ȣ��Ʈ   ����) : %d \n", ntohs(servent->s_port));

  return 0;
}