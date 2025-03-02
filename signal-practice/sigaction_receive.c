#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigusr1_handler(int sig, siginfo_t *info, void *ucontext) {
    printf("Got SIGUSR1 signal\n");

    // sigqueue()로 보낸 시그널인지 확인
    if (info->si_code & SI_QUEUE) {
        printf("siqqueue() API sent %d\n", info->si_int);
    }
    else {
        printf("Any other process sent\n");
    }
}

int main(int argc, char **argv) {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));

    act.sa_sigaction = sigusr1_handler;
    act.sa_flags = SA_SIGINFO; // sa_sigaction에 정의된 함수를 호출하겠다! 

    sigaction(SIGUSR1, &act, NULL);

    while(1) {
        sleep(1);
    }

    return 0;
}