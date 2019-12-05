#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int arg, char * argv[]) {
    int i, j, p;
    for (i = 0; i < 3; i++)
        if ((p = fork()) == 0) {
            printf("i =% d \n", i);
            j = 0;
            while ((j < i) && ((p = fork()) == 0))
                j++;
            if (p == 0)
                printf("j =% d \n", j);
            else
                wait(NULL);  // modification
            exit(j);
        }
    printf("wait for processes\n");
    for (i = 0; i < 3; i++) {
        wait(NULL);   // modification
    }
    printf("end of all processes\n");
    return (0);
}