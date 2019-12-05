#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define N 5

void usr1hd(int sig){
	printf("I am in wave 1\n");
}

void usr2hd(int sig){
	printf("I am in wave 2\n");
}

void inthd(int sig){
	printf("I am in wave 3\n");
}


int main(void){
	sigset_t sig;
	struct sigaction sigact_usr1;
	struct sigaction sigact_usr2;
	struct sigaction sigact_int;
	sigaddset(&sig, SIGUSR1);
	sigprocmask(SIG_SETMASK, &sig, NULL);

	sigfillset(&sig);
	sigdelset(&sig, SIGUSR1);
	sigact_usr1.sa_mask = sig;
	sigact_usr1.sa_flags = 0;
	sigact_usr1.sa_handler = usr1hd;
	sigaction(SIGUSR1, &sigact_usr1, NULL);

	sigfillset(&sig);
	sigdelset(&sig, SIGUSR2);
	sigact_usr2.sa_mask = sig;
	sigact_usr2.sa_flags = 0;
	sigact_usr2.sa_handler = usr2hd;
	sigaction(SIGUSR2, &sigact_usr2, NULL);

	sigfillset(&sig);
	sigdelset(&sig, SIGINT);
	sigact_int.sa_mask = sig;
	sigact_int.sa_flags = 0;
	sigact_int.sa_handler = inthd;
	sigaction(SIGINT, &sigact_int, NULL);

	pid_t pid;
	int i;
	printf("P0 (PP) starts\n");
	for (i = 0; i < N; i++){
		pid = fork();
		if (pid == -1){
			perror("fork error");
			exit(1);
		}else if (pid == 0){
			printf("P%d starts\n", i+1);
		}else {
			sigfillset(&sig);
			sigdelset(&sig, SIGUSR1);
			sigsuspend(&sig);
			if (i != 0){
				printf("prepare for wave 2\n");
				kill(getppid(), SIGUSR1);
				sigfillset(&sig);
				sigdelset(&sig, SIGUSR2);
				sigsuspend(&sig);
			}
			printf("P%d finish wave 1\n", i);
			//finish wave 1, prep wave 2


			//PP starts wave 2
			kill(pid, SIGUSR2);
			printf("P%d finish wave 2\n", i);
			sigfillset(&sig);
			sigdelset(&sig, SIGINT);
			sigsuspend(&sig);
			//finish wave 2

			if (i != 0){
				kill(getppid(), SIGINT);
				exit(EXIT_SUCCESS);
			}else{
				printf("end of programming\n");
				exit(EXIT_SUCCESS);
			}
		}
	}
	printf("I am the final child P%d (LL), now I start wave 1\n", i);
	kill(getppid(), SIGUSR1);

	sigfillset(&sig);
	sigdelset(&sig, SIGUSR2);
	sigsuspend(&sig);
	printf("I am the final child P%d (LL), now I start wave 3\n", i);

	kill(getppid(), SIGINT);
	exit(EXIT_SUCCESS);
}

