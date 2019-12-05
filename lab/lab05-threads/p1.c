#define XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>


#define N 5

void *rand_thread(void *arg);

int main(int argc, char** argv) {

    int i;
    int * pt_i, status;
    pthread_t tid[N];

    // for (i = 0; i < N; i++) {
    //     tid[i] = i * i + 10;
    // }

    for (i = 0; i < N; i++) {
        pt_i = (int*) malloc(sizeof(i));
        *pt_i = i;
        printf("i: %d\n", i);
        if (pthread_create(&(tid[i]), NULL, rand_thread, (void*)pt_i) != 0) {
            perror("error");
        } else {
            printf ( "pthread_create \n");
        }
        // free(pt_i);
        // printf("i: %d\n", i);
    }

    for (i = 0; i < N; i++) {
        pthread_join(tid[i], (void**) &status);
    }

    return 0;
}

void *rand_thread(void *arg) {
    // printf("%s Argument received, tid:%d \n", (char * ) arg, (int) pthread_self());
    printf("tid: %d\t i: %d\n", (int)pthread_self(), *(int*)arg);
    pthread_exit ((void *) 0); 
    // return NULL;
}

// void * test(void * arg) {
//     int i;
//     printf("%s Argument received, tid:%d \n", (char * ) arg, (int) pthread_self());
//     for (i = 0; i < 10; i++);
//     printf("end of secondary thread %d\n", (int) pthread_self());
//     return NULL;
// }

// int main(int argc, char ** argv) {
//     pthread_t tid;
//     if (pthread_create( & tid, NULL, test, "HELLO") != 0) {
//         perror("pthread_create\n");
//         exit(1);
//     }
//     sleep(3);
//     printf("end of main thread\n");
//     return EXIT_SUCCESS;
// }