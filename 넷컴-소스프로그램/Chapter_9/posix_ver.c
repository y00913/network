//--------------------------------------------------------------
// 파일명 : posix_ver.c
// 동  작 : 현재 시스템에 설치되어 있는 라이브러리 버전을 출력
// 컴파일 : gcc -o posix_lib_ver posix_lib_ver.c
// 실  행 : posix_lib_ver
//--------------------------------------------------------------

 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>

  int main() {
      long version = sysconf( _SC_VERSION );
      printf("Posix version : %ld\n", version);
      if ( version >= 199506L)
         printf("이 시스템에서는 Posix 라이브러리 사용이 가능합니다\n");
      else
         printf("이 시스템에서는 Posix1003.1c 스레드를 지원하지 않습니다\n");
      return 0;
  }