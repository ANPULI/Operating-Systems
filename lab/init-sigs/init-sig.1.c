#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>

int main () {
	pause ();
	printf ("1st signal\n");
	pause ();
	printf ("2nd signal\n");
	return 0;
}
