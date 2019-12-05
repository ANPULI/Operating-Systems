#define _XOPEN_SOURCE 700 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>

#define N_CHILDREN 5

void recur(i) {
    if (i<N_CHILDREN && fork()==0) {
        i++;
        printf("i am a process %d\n",i);
        recur(i);
    }
}

int main() {
    int i = 0;
    recur(i);
    return 0;
}