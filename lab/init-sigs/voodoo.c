#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main () {
		
	printf("PERE: %d\n", getpid());
	if (fork() != 0) {
		pause();
		pause();
		wait(0);
		// pause();
	} else {
		// pause();
		printf("FILS: %d\n", getpid());
		pause();
	}
	printf("%d> Fin\n", getpid());
	return 0;
}
