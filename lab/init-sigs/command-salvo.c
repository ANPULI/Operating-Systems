/**** spy.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define _XOPEN_SOURCE 700

#define BUFSZ 1024
#define WDSZ 24
#define NBUF 3

sigset_t sig;
struct sigaction sigact;
sigset_t sig_proc;

void sigint_handler (int sig) {
	printf ("SIGINT received\n");
}

int cut_down(char *buf, char **cmd) {
	char* s = strtok(buf, " \n");
	int i = 0;
	while (s != NULL) {
		cmd[i] = (char*) malloc(sizeof(char) * WDSZ);
		strcpy(cmd[i], s);
		s = strtok(NULL, " \n");
		i++;
	}
	cmd[i] = NULL;
	return i;
}


int main(int argc, char **argv)
{

    int or = 1, pid, command_size, i, j;
	char buf[BUFSZ];
	char *command[WDSZ];
	int childrenPID[NBUF+1];

	sigfillset (&sig);
	sigdelset (&sig, SIGINT);
	sigprocmask (SIG_SETMASK, &sig, NULL);
	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	sigact.sa_handler = SIG_DFL;
	sigaction (SIGINT, &sigact, NULL);

	
	j = 0;
    while (or > 0) 
    {
		for (i = 0; i < BUFSZ ; i++)
			buf[i] = '\0';
        if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
			perror("read");
		else if (or > 0) {
			command_size = cut_down(buf, (char**)command);
			/* TO DO :
					-> EXECUTION OF THE COMMAND BY A CHILD
					-> PARENT WAITS UNTIL COMPLETION AND RELEASES MEMORY */
			if ( (pid=fork()) ==0) {
				sigact.sa_handler = sigint_handler;
				sigaction (SIGINT, &sigact, NULL);
				sigfillset (&sig_proc);
				sigdelset (&sig_proc, SIGINT);
				sigsuspend (&sig_proc);
				if (command_size == 2) {
					// printf("We are using execlp");
					// printf("%s\n", command[0]);
					// printf("%s\n", command[1]);
					execlp(command[0], command[0], command[1], NULL);
				} else {
					execvp(command[0], &command[0]);					
				}
				exit(0);
			} else {
				childrenPID[j] = pid;
				// wait(0);
				// free(command);
			}
		}
		j++;
		if (j == NBUF)
			break;
    }

	for (i = 0; i < NBUF; i++) {
		kill(childrenPID[i], SIGINT);
		printf("%d \n", childrenPID[i]);
		wait(0);
	}
	
	return 0;
}
