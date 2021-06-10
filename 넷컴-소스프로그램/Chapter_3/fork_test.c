//--------------------------------------------------------------
// 파일명 : fork_test.c
// 기  능 : fork() 시스템 콜 사용 예
// 컴파일 : cc -o fork_test fork_test.c
// 사용법 : fork_test
//--------------------------------------------------------------

#include <sys/types.h>
#include <unistd.h>
int global_var = 0;         // 전역 변수 선언

int main(void) {
   pid_t pid;
   int local_var = 0;          // 지역 변수 선언
   if((pid = fork()) < 0) {
         printf("fork error\n");
         exit(0);
   }
   else if (pid == 0) { // 자식 프로세스
         global_var++;
         local_var++;
         printf("CHILD - my pid is %d and parent's pid is %d\n",
                                           getpid(), getppid());
   }
   else {

          // 부모 프로세스
          sleep(2);     // 2초 쉰다
          global_var += 5;
          local_var += 5;
          printf("PARENT - my pid is %d, child's pid is %d\n",
                                                   getpid(), pid);
    }
    printf("\t global var : %d\n", global_var);
    printf("\t local var : %d\n", local_var);
}