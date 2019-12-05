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
    //     printf("P%d; PID: %d, PPID: %d\n", i, getpid(), getppid());
    //     // wait(0);  the initial parent will not wait
    // }
    // if (i < N_CHILDREN)
    //     wait(0);

    for (i = 0; i < N_CHILDREN; i++) {
        if (fork() > 0) {
            printf("P%d; PID: %d, PPID: %d\n", i, getpid(), getppid());
            wait(0);
            exit(1);
        }
    } 
    return 0;
}

