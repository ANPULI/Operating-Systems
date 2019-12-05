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
int PPpid;  // pid of PP
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
    sigfillset(&sig_proc);
    sigprocmask(SIG_SETMASK, &sig_proc, NULL);

    PPpid = getpid();
    printf("ppid of PP: %d\n", getppid());
    printf("pid of PP: %d\n", PPpid);

    for (i = 1; i < N; i++) {
        if ( (child_pid = fork()) > 0 ) {
            // suspend these processes
            sigfillset(&sig_proc);
            sigdelset(&sig_proc, SIGUSR1);
            sigsuspend(&sig_proc);
            break;
        }
    }

    if (i == N) {
        printf("\n=================WAVE1====================\n");
        printf("This is process %d\n", i);
        printf("\tI am going to send SIGUSR1 to my parent\n");
        // printf("iiiiiii\n");

        sigfillset(&sig_proc);
        sigprocmask(SIG_SETMASK, &sig_proc, NULL);
        kill(getppid(), SIGUSR1);

        // printf("iiiiiii\n");
        sigdelset(&sig_proc, SIGUSR2);
        sigsuspend(&sig_proc);
        
    } else {
        // sigfillset(&sig_proc);
        // sigdelset(&sig_proc, SIGUSR1);
        // sigsuspend(&sig_proc);
    }

    return 0;
}

void sig_hand_usr1 (int sig) {
    // send signal from bottom to top
    printf("This is process %d\n", i);
    printf ("\treceived signal %d \n", sig);
    if (i == 1) {
        printf("\tI am going to send SIGUSR2 to my child\n\n");
        printf("=================WAVE2====================\n");
        kill(child_pid, SIGUSR2);
        // get suspended by SIGNIT
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGINT);
        sigsuspend(&sig_proc);
    } else {
        printf("\tI am going to send SIGUSR1 to my parent\n");
        kill(getppid(), SIGUSR1);
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGUSR2);
        sigsuspend(&sig_proc);
    }
}

void sig_hand_usr2 (int sig) {
    // send signal from top to bottom
    printf("This is process %d\n", i);
    printf ("\treceived signal %d \n", sig);
    if (i == N) {
        printf("\tI am going to send SIGINT to my parent\n\n");
        printf("=================WAVE3====================\n");
        kill(getppid(), SIGINT);
        // exit(0);
    } else {
        printf("\tI am going to send SIGUSR2 to my child\n");
        kill(child_pid, SIGUSR2);
        // get suspended by SIGINT
        sigfillset(&sig_proc);
        sigdelset(&sig_proc, SIGINT);
        sigsuspend(&sig_proc);
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
        printf("=============End of the wave=============\n\n");
    }
}