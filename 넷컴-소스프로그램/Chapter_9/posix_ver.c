//--------------------------------------------------------------
// ���ϸ� : posix_ver.c
// ��  �� : ���� �ý��ۿ� ��ġ�Ǿ� �ִ� ���̺귯�� ������ ���
// ������ : gcc -o posix_lib_ver posix_lib_ver.c
// ��  �� : posix_lib_ver
//--------------------------------------------------------------

 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>

  int main() {
      long version = sysconf( _SC_VERSION );
      printf("Posix version : %ld\n", version);
      if ( version >= 199506L)
         printf("�� �ý��ۿ����� Posix ���̺귯�� ����� �����մϴ�\n");
      else
         printf("�� �ý��ۿ����� Posix1003.1c �����带 �������� �ʽ��ϴ�\n");
      return 0;
  }