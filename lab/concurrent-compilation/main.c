#include "tata.h"
#include "titi.h"
#include "toto.h"
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // char * path = "/media/sf_Work/lab/concurrent-compilation/";
    char * path = "";
    int i;
    char * target;
    char * list[argc];


    for (i = 1; i < argc; i++) {
        if (fork() == 0) {
            char x[100], y[100], buffer[10];
            target = argv[i];
            strcpy(x, path);
            strcpy(y, path);
            memcpy(buffer, target, strlen(target)-1);
            buffer[strlen(target)-1] = 'o';
            buffer[strlen(target)] = '\0';
            strcat(x, buffer);
            strcat(y, target);

            printf("%s\n", x);
            printf("%s\n", y);
            // if (execl("/usr/bin/wc", "wc", "-w", "/media/sf_Work/lab/concurrent-compilation/main.c", (char *)NULL) == -1){
            //     perror("execl");
            // }
            if (execl("/usr/bin/gcc", "gcc", "-c", "-o", x, y, (char *)NULL) == -1){
                perror("execl");
            }
        }
    }

    // gcc .
    for (i = 1; i < argc; i++) {
        wait(0);
    }

    return 0;
}