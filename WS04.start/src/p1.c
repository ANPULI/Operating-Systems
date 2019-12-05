#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int sec = 2;
sigset_t sig_proc;
struct sigaction action;

void sig_hand (int sig) {

    printf ("received signal %d \n", sig);
}

unsigned int mysleep(int sec) {
    alarm(sec);
    sigsuspend(&sig_proc);
    // pause();
    return alarm(0);
}

int main(int argc, char** argv) {
    int i;
    sigemptyset(&sig_proc);
        action.sa_mask = sig_proc;
        action.sa_flags = 0;
        action.sa_handler = sig_hand;
    for (i = 1; i < NSIG; i++) {
        sigaction(i, &action, 0);
    }
    
    printf("Before calling mysleep\n");
    printf("I am going to sleep for %d seconds\n", sec);
    i = mysleep(sec);
    // sleep(sec);
    printf("After calling mysleep\n");
    printf("return value: %d\n", i);
        action.sa_handler = SIG_DFL;
    for (i = 1; i < NSIG; i++) {
        sigaction(i, &action, 0);
    }
    return 0;
}