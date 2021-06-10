//--------------------------------------------------------------
// ���ϸ� : fork_test.c
// ��  �� : fork() �ý��� �� ��� ��
// ������ : cc -o fork_test fork_test.c
// ���� : fork_test
//--------------------------------------------------------------

#include <sys/types.h>
#include <unistd.h>
int global_var = 0;         // ���� ���� ����

int main(void) {
   pid_t pid;
   int local_var = 0;          // ���� ���� ����
   if((pid = fork()) < 0) {
         printf("fork error\n");
         exit(0);
   }
   else if (pid == 0) { // �ڽ� ���μ���
         global_var++;
         local_var++;
         printf("CHILD - my pid is %d and parent's pid is %d\n",
                                           getpid(), getppid());
   }
   else {

          // �θ� ���μ���
          sleep(2);     // 2�� ����
          global_var += 5;
          local_var += 5;
          printf("PARENT - my pid is %d, child's pid is %d\n",
                                                   getpid(), pid);
    }
    printf("\t global var : %d\n", global_var);
    printf("\t local var : %d\n", local_var);
}