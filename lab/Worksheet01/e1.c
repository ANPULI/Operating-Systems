#define _XOPEN_SOURCE 700 
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h>
#include <stdio.h> 
#include <stdlib.h>

#define N_CHILDREN 5

int main() {
    int i;
    // for (i = 0; (i < N_CHILDREN) && (fork()==0); i++) {
    //     printf("I am a process; PID: %d, PPID: %d\n", getpid(), getppid());
    // }
    for (i = 0; i < N_CHILDREN; i++) {
        if (fork()==0) {
            printf("P%d; PID: %d, PPID: %d\n", i, getpid(), getppid());
            exit(1);
        }
    }
    for (i = 0; i < N_CHILDREN; i++) {
        wait(0);
    }
    return 0;
}