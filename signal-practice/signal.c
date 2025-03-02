#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/*
- 사용법
1. 터미널에서 kill -TERM pid / kill -TERM `pidof signal` / killall -TERM signal 입력 
    -> 핸들러 실행
2. 터미널에서 Ctrl+C 입력 혹은 kill -INT pid 입력
    -> 무시
*/

void sigterm_handler(int signum) {
    printf("Got SIGTERM signal...\n");
}

int main(int argc, char **argv) {
    sigset_t set; // signal set

    // signal set 구성하기
    sigemptyset(&set); //  초기화
    sigaddset(&set, SIGINT); // signal set에 SIGINT 시그널 등록

    sigprocmask(SIG_SETMASK, &set, NULL); // 해당 signal set을 blocked signal로 설정

    signal(SIGTERM, sigterm_handler); // SIGTERM signal에 대한 핸들러 등록

    printf("I'm waiting signal...\n");
    while(1) {
        sleep(1);
    }

    return 0;
}
