#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sig_hand(int sig) {
    printf("Signal delivered: %d\n", sig);
}

int main (int argc, char* argv []) {
    
    int i;
    sigset_t  sig_rec ;	/* list of blocked singals */
    sigset_t  sig_pend;	/* list of pending signals */
    sigset_t sig_proc;
    struct sigaction act_int;
    struct sigaction act_quit;

    sigemptyset(&sig_proc);

    
    
    sigfillset(&sig_rec);
    sigprocmask (SIG_SETMASK, &sig_rec, NULL);

    sigemptyset (&sig_rec);
    sigaddset (&sig_rec,SIGINT);
    sigaddset (&sig_rec,SIGQUIT);

    // sigemptyset(&action.sa_mask);
    /* mask SIGINT and SIGQUIT */
    sigprocmask (SIG_SETMASK, &sig_rec,NULL);
    
    sleep (5);
    
    /* retrieve pending signals */
    sigpending (&sig_pend);
    if (sigismember ( &sig_pend,SIGINT) )
        printf("SIGINT received\n");
    if (sigismember ( &sig_pend,SIGQUIT) )
        printf("SIGQUIT received\n");
    
    for (i=1; i <NSIG; i++)
        if (sigismember ( &sig_pend,i) )
            printf("%d",i);
    printf ("\n");



    sigemptyset(&sig_proc);
        act_int.sa_mask = sig_proc;
        act_int.sa_flags = 0;
        act_int.sa_handler = sig_hand;
    sigaction(SIGINT, &act_int, 0);
    sigaction(SIGQUIT, &act_int, 0);

    sigfillset(&sig_proc);
    sigdelset(&sig_proc, SIGINT);
    sigdelset(&sig_proc, SIGQUIT);
    sigsuspend(&sig_proc);

    // sigdelset (&sig_rec,SIGINT);
    // sigdelset (&sig_rec,SIGQUIT);
    // sigprocmask (SIG_SETMASK, &sig_rec,NULL);

    
    printf ("End program \n");
    
    return EXIT_SUCCESS;
}
