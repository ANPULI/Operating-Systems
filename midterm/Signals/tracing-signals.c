#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>


int i;

void sig_hand(int sig) {
    if (SIGINT == sig) {
        i += 2;
    } else if (SIGUSR1 == sig) {
        i -= 2;
    } else if (SIGUSR2 == sig) {
        i *= 2;
    }
}

int main(int argc, char** argv) {

    i = 1;
    sigset_t sig_proc;
    signal(SIGINT, sig_hand);
    signal(SIGUSR1, sig_hand);
    signal(SIGUSR2, sig_hand);

    printf("Please help me out of the infinite loop!\n");
    while (i) {
        
    }
    printf("Thank you! Now I can safely go now.\n");

    return 0;
}