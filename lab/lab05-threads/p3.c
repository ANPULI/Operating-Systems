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

int sum, flag = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *rand_thread(void *arg);
void *print_thread(void *arg);

int main(int argc, char** argv) {

    int i, status;
    int * pt_i;
    pthread_t tid[N+1];

    sum = 0;

    for (i = 0; i < N+1; i++) {
        pt_i = (int*) malloc(sizeof(i));
        *pt_i = i;
        printf("i: %d\t", i);
        if (i < N) {
            if (pthread_create(&(tid[i]), NULL, rand_thread, (void*)pt_i) != 0) {
                perror("error");
            } else {
                printf ( "rand_thread_create \n");
            }
        } else {
            if (pthread_create(&(tid[i]), NULL, print_thread, (void*)pt_i) != 0) {
                perror("error");
            } else {
                printf ( "print_thread_create \n");
            }
        }
    }


    for (i = 0; i < N+1; i++) {
        if (i < N) {
            pthread_detach(tid[i]);
        } else {
            pthread_join(tid[i], (void**) &status);
        }
    }

    printf("main print sum: %d\n", sum);
    return 0;
}

void *rand_thread(void *arg) {
    // printf("%s Argument received, tid:%d \n", (char * ) arg, (int) pthread_self());
    int i = * ((int *) arg);
    int random_val = (int) (10 * ((double) rand ()) / RAND_MAX);
    printf("tid: %d\t i: %d\t random_val: %d\n", (int)pthread_self(), i, random_val);
    pthread_mutex_lock(&mutex);
    sum += random_val;
    flag += 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit ((void *) 0); 
    // return NULL;
}

void *print_thread(void *arg) {
    pthread_mutex_lock(&mutex);
    while (flag < N) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("thread print sum: %d\n", sum);
    pthread_mutex_unlock(&mutex);
    pthread_exit((void *) 0);
}