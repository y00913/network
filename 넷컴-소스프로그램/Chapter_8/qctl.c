//--------------------------------------------------------------
// 파일명 : qctl
// 동  작 : msgctl()를 이용해 메시지큐의 정보를 출력
// 컴파일 : gcc -o qctl  qctl.c
// 실  행 : qctl  msqkey
//--------------------------------------------------------------

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int msq_remove(int qid);	// 메시지큐 제거 함수
int view_qinfo(int qid);        // 메시지큐 정보출력 함수

int main(int argc, char **argv) {
    int qid;
    key_t key;

    if(argc != 2) {
        printf("USAGE : %s   msqkey\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    key = atoi(argv[1]);

    // 기존에 생성된 메시지큐 ID 얻기
    if((qid=msgget(key, 0)) < 0) {
      perror("msgget fail ");
      exit(0);
    }

    view_qinfo(qid);              // 메시지큐 정보 출력
    msq_remove(qid);              // 메시지큐 제거
    exit(EXIT_SUCCESS);
}

// 메시지큐 정보 출력함수
int view_qinfo(int qid) {
    struct msqid_ds  buf;
    struct ipc_perm  *pm;

    // 메시지큐 객체 얻기
    if((msgctl(qid, IPC_STAT, &buf))<0) {
        perror("msgctl");
        return -1;
    }
    pm = &buf.msg_perm;
    printf("큐의 최대 바이트수 : %d\n",buf.msg_qbytes);
    printf("큐의 유효 사용자 UID : %d\n",pm->uid);
    printf("큐의 유효 사용자 GID : %d\n",pm->gid);
    printf("큐 접근권한 : 0%o\n",pm->mode);
    return 0;
}

// 메시지큐 제거 함수
int msq_remove(int qid) {
    if((msgctl(qid, IPC_RMID, NULL)) < 0) {
        perror("msgctl");
        return -1;
    }
    printf("메시지큐 %d 삭제됨\n", qid);
    return 0;
}