#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>


sigset_t sig_proc;
sigset_t sig_rec;
sigset_t sig_pend;  //pending signals
struct sigaction action;
int nsig;
int k;

void sig_hand_parent (int sig);
void sig_hand_child (int sig);
void wait_child();
void send_sig_p2c(pid_t* pid_child);
void send_sig_c2p(int i);
void calc1 ();
void calc2 ();


int main (int argc, char * argv []) { 

    int i = 0; 
    pid_t pid_child [2]; 
    nsig = 0;

    sigemptyset(&sig_proc);
        action.sa_mask = sig_proc;
        action.sa_flags = 0;
        action.sa_handler = sig_hand_parent;
    sigaddset(&sig_proc, SIGUSR1);
    sigaddset(&sig_proc, SIGUSR2);
    sigprocmask(SIG_SETMASK, &sig_proc, NULL);

    while ( (i < 2) && ((pid_child[i] = fork()) != 0) )   {
        i ++; 
    }
    k = i;
    
    if (i == 2) {
        // parent
        // action.sa_handler = sig_hand_parent;
        sigaction(SIGUSR1, &action, 0);
        sigaction(SIGUSR2, &action, 0);
        
        wait_child();
        calc1();
        send_sig_p2c(pid_child);

        wait_child();
        calc2();
        send_sig_p2c(pid_child);
    } else {
        action.sa_handler = sig_hand_child;
        if (i == 0) 
            sigaction(SIGUSR1, &action, 0);
        else
            sigaction(SIGUSR2, &action, 0);
        calc1();
        send_sig_c2p(i);
        
        calc2();
        send_sig_c2p(i);
    }

    // sigaction(SIGUSR1, &action, 0);
    // sigfillset(&sig_proc);
    // sigdelset(&sig_proc, SIGUSR1);

    // calc1 ();
    // if (i == 2) {
    //     kill(pid_child[0], SIGUSR1);
    //     kill(pid_child[1], SIGUSR1);
    //     sigsuspend(&sig_proc);
    //     sigsuspend(&sig_proc);
    // }
    // calc2 (); 
    printf ( "End Process %d \n", i);  

    return EXIT_SUCCESS ; 

} 

void sig_hand_parent (int sig) {
    printf ("Parent received signal %d \n", sig);
    nsig++;
}

void sig_hand_child (int sig) {
    printf ("Child %d received signal %d \n", k, sig);
}

void wait_child() {
    // int SIGUSRS[2] = {SIGUSR1, SIGUSR2};
    // int i;
    // sigaddset(&sig_proc, SIGUSR1);
    // sigaddset(&sig_proc, SIGUSR2);
    // sigprocmask(SIG_SETMASK, &sig_proc, NULL);
    // if (sigismember(&sig_pend, SIGUSR1)) {
    //     if (sigismember(&sig_pend, SIGUSR2)) {
    //         // normal
    //     } else {
    //         // wait for C2
    //         sigfillset(&sig_rec);
    //         sigdelset(&sig_rec, SIGUSR2);
    //         sigsuspend(&sig_rec);
    //         sigpending(&sig_pend);
    //     }
        
    // }
    // printf("This line is reached 1\n");

    // sleep(10);
    sigemptyset(&sig_rec);
    sigprocmask(SIG_SETMASK, &sig_rec, NULL);
    while (nsig < 2) {
        sigfillset(&sig_rec);
        sigdelset(&sig_rec, SIGUSR1);
        sigdelset(&sig_rec, SIGUSR2);
        sigsuspend(&sig_rec);
    }

    // for (i = 0; i < 2; i++) {
    //     sigpending(&sig_pend);
    //     if ( !sigismember(&sig_pend, SIGUSRS[i]) ){
    //         sigfillset(&sig_rec);
    //         sigdelset(&sig_rec, SIGUSRS[i]);
    //         sigsuspend(&sig_rec);
    //         sigpending(&sig_pend);
    //     }
    //     printf("This line is reached %d\n", i+2);
    // }
    // printf("This line is reached 4\n");
    sigemptyset(&sig_proc);
    // printf("This line is reached 5\n");
    sigprocmask(SIG_SETMASK, &sig_proc, NULL);
    // printf("This line is reached 6\n");

    nsig = 0;
}

void send_sig_p2c(pid_t* pid_child) {
    int SIGUSRS[2] = {SIGUSR1, SIGUSR2};
    int i;
    for (i = 0; i < 2; i++) {
        kill(pid_child[i], SIGUSRS[i]);
    }
}

void send_sig_c2p(int i) {
    int SIGUSRS[2] = {SIGUSR1, SIGUSR2};
    kill(getppid(), SIGUSRS[i]);
    printf("Send SIGUSR%d to parent\n", i+1);
    sigfillset(&sig_rec);
    sigdelset(&sig_rec, SIGUSRS[i]);
    sigsuspend(&sig_rec);
}

void calc1 () { 
    int i; 
    for (i = 0; i <1E8; i ++); 
    printf("calc 1 exec by %d\n", k);
} 


void calc2 () { 
    int i; 
    for (i = 0; i <1E8; i ++); 
    printf("calc 2 exec by %d\n", k);
} 
