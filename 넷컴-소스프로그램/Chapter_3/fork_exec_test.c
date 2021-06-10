//--------------------------------------------------------------
// ���ϸ� : fork_exec_test.c
// ��  �� : fork�� exec �Լ��� �����ϱ� ���� ����
// ������ : gcc -o fork_exec_test fork_exec_test.c
// ��  �� : fork_exec_test
//--------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void child_start();

int main(int argc, char **argv) {
    pid_t pid;
    int child_status, child_return;

    puts("\t parent process start..............");
    if( (pid=fork()) < 0) {           // ���� ó��
        perror("fork failed : ");
        exit(0);
    }

    else if (pid == 0)              // �ڽ� ���μ���
        child_start();
    else if( pid > 0)               // �θ� ���μ���
        printf("\n\t** parent : [my pid: %d] my child pid = %d\n",getpid(), pid);
    return 0;
}

void child_start() {
    puts("\t child process start...");
    printf("\t** child : [my pid: %d] my parent pid = %d\n",getpid(), getppid());
    printf("\n\t** exec() �Լ��� ls ����� �����մϴ�\n");
    execlp("ls", "ls", NULL);

    // �Ʒ��� ����ȴٸ� exec �Լ����� ������ �� �����
    perror("exec error at child: ");
    exit(0);
}