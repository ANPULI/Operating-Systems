#define _XOPEN_SOURCE 700 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <wait.h>

#define N 5


int main() {
    int i, a, b, p;
    a = 1; b = 0;

    for (i = 0; i < N; i++) {
        p = fork();
        if ( p == -1) {
            exit(1);
        } else if (p > 0) {
            wait(0);
            break;
        } else if (p == 0) {
            printf("level %d: %d\n", i+1, a);
            a += b;
            b = a - b;
        }
    }

    return 0;
}
