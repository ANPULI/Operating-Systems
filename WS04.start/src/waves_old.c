#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

#define N 3

sigset_t sig_proc;
struct sigaction action;
// int PPpid;  // pid of PP
int i;  // index of process
int child_pid;

void sig_hand_usr1 (int sig);
void sig_hand_usr2 (int sig);
void sig_hand_int (int sig);

int main(int argc, char** argv) {

    // set sig handlers
    sigemptyset(&sig_proc);
        action.sa_mask = sig_proc;
        action.sa_flags = 0;
        action.sa_handler = sig_hand_usr1;
    sigaction(SIGUSR1, &action, 0);
        action.sa_handler = sig_hand_usr2;
    sigaction(SIGUSR2, &action, 0);
        action.sa_handler = sig_hand_int;
    sigaction(SIGINT, &action, 0);

    // PPpid = getpid();
    // printf("ppid of PP: %d\n", getppid());
    printf("pid of PP: %d\n", PPpid);

    for (i = 1; i < N; i++) {
        if ( (child_pid = fork()) > 0 ) {
            break;
        }
    }

    // suspended for SIGUSR1
    if (i == N) {
        printf("=================WAVE1====================\n");
        printf("This is process %d\n", i);
        printf("\tI am going to send SIGUSR1 to my parent\n");
        kill(getppid(), SIGUSR1);
    } else {
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGUSR1);
        sigsuspend(&sig_proc);
    }
    
    // suspended for SIGUSR2
    if (i == 1) {
        kill(child_pid, SIGUSR2);
    } else {
        // sigfillset(&sig_proc);
        // sigdelset(&sig_proc, SIGUSR2);
        // sigsuspend(&sig_proc);
    }

    // suspended for SIGINT
    if (i == N) {
        printf("Am I in this stage?\n");
        kill(getppid(), SIGINT);
        exit(0);
    } else {
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGINT);
        sigsuspend(&sig_proc);
        // wait(0);
    }

    return 0;
}

void sig_hand_usr1 (int sig) {
    // send signal from bottom to top
    printf("This is process %d\n", i);
    printf ("\treceived signal %d \n", sig);
    if (i != 1) {
        printf("\tI am going to send SIGUSR1 to my parent\n");
        kill(getppid(), SIGUSR1);
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGUSR2);
        sigsuspend(&sig_proc);
    } else {
        printf("\tI am going to send SIGUSR2 to my child\n\n");
        printf("=================WAVE2====================\n");
    }
}

void sig_hand_usr2 (int sig) {
    // send signal from top to bottom
    printf("This is process %d\n", i);
    printf ("\treceived signal %d \n", sig);
    if (i != N) {
        printf("\tI am going to send SIGUSR2 to my child\n");
        kill(child_pid, SIGUSR2);
        // sigfillset(&sig_proc);
        // sigdelset(&sig_proc, SIGINT);
        // sigsuspend(&sig_proc);
    } else {
        printf("\tI am going to send SIGINT to my parent\n\n");
        printf("=================WAVE3====================\n");
    }
}

void sig_hand_int (int sig) {
    // send signal from bottom to top
    printf("This is process %d\n", i);
    printf ("\treceived signal %d \n", sig);
    if (i != 1) {
        printf("\tI am going to send SIGINT to my parent\n");
        kill(getppid(), SIGINT);
    } else {
        printf("=============End of the wave=============\n");
    }
}