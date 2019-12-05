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
int k;
int nsig;
// int sig_queue[2];

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
    // sig_queue[0] = 0; sig_queue[1] = 0;

    sigemptyset(&sig_proc);
        action.sa_mask = sig_proc;
        action.sa_flags = 0;
        action.sa_handler = sig_hand_parent;

    while ( (i < 2) && ((pid_child[i] = fork()) != 0) )   {
        i ++; 
    }
    k = i;
    
    if (i == 2) {
        // parent
        sigaction(SIGUSR1, &action, 0);
        
        wait_child();
        calc1();
        send_sig_p2c(pid_child);

        // wait_child();
        calc2();
        // send_sig_p2c(pid_child);
        for (i = 0; i < 2; i++)
            wait(0);
    } else {
        action.sa_handler = sig_hand_child;
        sigaction(SIGUSR1, &action, 0);

        calc1();
        send_sig_c2p(i);
        
        calc2();
        // send_sig_c2p(i);
    }

    printf ( "End Process %d \n", i);  

    return EXIT_SUCCESS ; 

} 

void sig_hand_parent (int sig) {
    printf ("Parent received signal %d \n", sig);
    // if (sig_queue[0] == 0)
    //     sig_queue[0] = 1;
    // else 
    //     sig_queue[1] = 1;
    nsig ++;
}

void sig_hand_child (int sig) {
    printf ("Child %d received signal %d \n", k, sig);
}

void wait_child() {
    // while (sig_queue[0] == 0 || sig_queue[1] == 0) {
    while (nsig < 2) {
        sigfillset(&sig_rec);
        sigdelset(&sig_rec, SIGUSR1);
        sigsuspend(&sig_rec);
    }
    // sig_queue[0] = 0; sig_queue[1] = 0;
    nsig = 0;
}

void send_sig_p2c(pid_t* pid_child) {
    int i;
    for (i = 0; i < 2; i++) {
        kill(pid_child[i], SIGUSR1);
    }
}

void send_sig_c2p(int i) {
    kill(getppid(), SIGUSR1);
    printf("Send SIGUSR%d to parent\n", 1);
    sigfillset(&sig_rec);
    sigdelset(&sig_rec, SIGUSR1);
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
