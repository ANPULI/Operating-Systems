#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

#define N 4
// #define M 100

int i, j, k;
// int sig_queue[M];
int nsig;
void mywait();
void sig_hand(int sig);

int main (int arg, char * argv []){

    pid_t pid;
    // pid_t child_pid[N];
    struct sigaction action;
    j = 0;
    nsig = 0;
    pid_t Ppid = getpid();
    // printf("The pid of Adam: %d\n", Ppid);

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sig_hand;
    sigaction(SIGUSR1, &action, 0);

    // parent created 4 children and print
    // 0,0   ---   3,0
    for (i = 0; i < N && (pid=fork()) != 0; i++) {
        printf ("i:%d j:%d \n", i,j);
        // child_pid[i] = pid;
    }

    if (pid != 0) {
        for (k = 0; k < N; k++) {
            // wait(0);
            mywait();
        }
    }

    if (((i%2) == 1) && (pid == 0)) {
        // child process: 1,3
        while ((j < i) && ((pid=fork()) == 0)) {
            // grandchild
            j++;
            printf ("i:%d j:%d \n", i,j);
        }
        if (pid != 0) {
            // wait(0);
            mywait();
        }
    }

    if (Ppid != getpid()) {
        // printf("My pid: %d \t Parent pid: %d\n", getpid(), getppid());
        kill(getppid(), SIGUSR1);
        // printf("My pid: %d \t Parent pid: %d\n", getpid(), getppid());
        // it is supposed to be ended !!
        // so the parent may die before reaching next line !!!
        return 0;
    }
    return 0;
}

void mywait() {
    // check if the sig_queue[0] == 1
    // if not -- suspend
    // if (sig_queue[0] == 0) {
    if (nsig == 0) {
        // suspend
        sigset_t sig_proc;
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGUSR1);
        sigsuspend(&sig_proc);
    }
    nsig--;
    // int i;
    // for (i = 0; i < M-1; i++)
    //     sig_queue[i] = sig_queue[i+1];
    // sig_queue[M-1] = 0;
}

void sig_hand(int sig) {
    // printf("Received sig %d\n", sig);
    // int i;
    // for (i = 0; i < M; i++) {
    //     if (sig_queue[i] == 0) {
    //         sig_queue[i] = 1;
    //         break;
    //     }
    // }
    nsig++;
}