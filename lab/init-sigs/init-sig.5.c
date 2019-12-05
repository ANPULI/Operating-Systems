#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact_int;
struct sigaction sigact_alrm;
int n_int = 0;
int n_alrm = 0;

void sigalrm_handler (int sig) {
	printf ("SIGALRM received\n");
	alarm (2);
}

void sigint_handler (int sig) {
	printf ("SIGINT received\n");
}


int main () {
	sigfillset (&sig);
	sigdelset (&sig, SIGINT);
	sigdelset (&sig, SIGALRM);
	sigprocmask (SIG_SETMASK, &sig, NULL);
	sigact_int.sa_handler = sigint_handler;
	sigaction (SIGINT, &sigact_int, NULL);
	sigact_alrm.sa_handler = sigalrm_handler;
	sigaction (SIGALRM, &sigact_alrm, NULL);
	alarm (2);
	pause();
	while (1) {
		sigsuspend (&sig);
	}
	return 0;
}
