//--------------------------------------------------------------
// 파일명 : mkq.c
// 동  작 : 메시지큐를 생성하고 메시지큐 ID를 출력
// 컴파일 : gcc -o mkq mkq.c
// 실  행 : mkq key_value
//--------------------------------------------------------------

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int qid;
    key_t key;

    if(argc!=2) {
        printf("Usage : %s  key\n",argv[0]);
        exit(1);
    }

    key = atoi(argv[1]);
    // 메시지큐 생성
    if((qid=msgget(key, IPC_CREAT | IPC_EXCL | 0666))<0) {
        perror("msgget fail");
        exit(EXIT_FAILURE);
    }

    // 메시지큐의 ID 출력
    printf("created queue id = %d\n", qid);

    // key 값으로 이미 생성된 메시지큐 ID 얻기
    qid = msgget(key, 0);
    printf("key is  = %d\n", key);
    printf("opened queue id = %d\n", qid);

    return 0;
}