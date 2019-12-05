/**** decoder.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <decoder.h>
#include <wait.h>


#define _XOPEN_SOURCE 700


int magicsq[3][3] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};


int check() {
	int i, j, sums[8];
	for (i = 0; i < 8; i++) 
		sums[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sums[i] += magicsq[i][j];
			sums[i+3] += magicsq[j][i];
			if (i == j) sums[6] += magicsq[i][j];
			if ((i+j) == 2) sums[7] += magicsq[i][j];
		}
	}
	int result = 1;
	i = 1;
	while ((i < 8) && (result == 1)) {
		if (sums[0] != sums[i])
			result = 0;
		i++;
	}
	return result;
}


void display() {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("  %d", magicsq[i][j]);
		printf("\n");
	}
	printf("\n");
}


int solve(int x) {
	/* CODE THAT EVERY CHILD PROCESS EXECUTES */
	magicsq[0][1] = x;
	int s = magicsq[0][0] + magicsq[0][1] + magicsq[0][2];
	magicsq[1][0] = s - magicsq[0][0] - magicsq[2][0];
	magicsq[1][2] = s - magicsq[0][2] - magicsq[2][2];
	magicsq[1][1] = s - magicsq[1][0] - magicsq[1][2];
	magicsq[2][1] = s - magicsq[2][0] - magicsq[2][2];
	return 0;
}

int main(int argc, char **argv)
{	
	/* PARENT PROCESS */
	int i;
	for (i = 0; i < 10; i++) {
		// printf("%d", i);
		if (fork() == 0) {
			solve(i);
			if (check() == 1)
				display();
			exit(EXIT_SUCCESS);
		}
	}

	for (i = 0; i < 10; i++)
		wait(NULL);
	
	return 0;
}
