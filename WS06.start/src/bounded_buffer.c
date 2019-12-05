/*
Question 3 - Bounded buffer with threads

    Write a program bounded-buffer that creates any number of the following two types of threads: producers and consumers. 
    All threads exchange data (integers ranging from 0 to 100) via an array with a maximum capacity of 100 integer values. 
    Access to the array is LIFO: producers push random values (0 < v ≤ 100) on top of the previous values, 
    while consumers pop values from the top and display them.

*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define NB_INTEGERS 100

int stack[NB_INTEGERS], flag_prod = 0, flag_cons = 0, count_prod = 0, count_cons = 0, NB_Producers, NB_Consumers;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;

void* thread_push(void* arg);
void* thread_pop(void* arg);
void print_stack(int stack[]);

int main(int argc, char** argv) {

    int i, status, *pt_i;
    NB_Producers = atoi(argv[1]); NB_Consumers = atoi(argv[2]);
    pthread_t tid_prod[NB_Producers], tid_cons[NB_Consumers];

    printf("check point 1\n");

    for (i = 0; i < NB_INTEGERS; i++) {
        stack[i] = 0;  // if there is a value, it should be > 0
    }

    printf("check point 2\n");

    for (i = 0; i < NB_Producers; i++) {
        pt_i = (int*) malloc(sizeof(int));
        *pt_i = i;
        if (pthread_create(&tid_prod[i], NULL, thread_push, (void*)pt_i) != 0) {
            perror("producer_create_fail");
            exit(1);
        }
    }

    printf("check point 3\n");

    for (i = 0; i < NB_Consumers; i++) {
        pt_i = (int*) malloc(sizeof(int));
        *pt_i = i;
        if (pthread_create(&tid_cons[i], NULL, thread_pop, (void*)pt_i) != 0) {
            perror("consumer_create_fail");
            exit(1);
        }
    }

    printf("check point 4\n");

    for (i = 0; i < NB_Producers; i++) {
        pthread_join(tid_prod[i], (void**) &status);
    }
    
    for (i = 0; i < NB_Consumers; i++) {
        pthread_join(tid_cons[i], (void**) &status);
    }
    
    return 0;
}

void* thread_push(void* arg) {
    int i, random_value, prod_id = *(int*)arg;
    count_prod++;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (stack[prod_id] == 0) {
            random_value = rand() % 100 + 1;
            printf("producer: %d\t push value: %d\n", *(int*)arg, random_value);
            stack[prod_id] = random_value;
            print_stack(stack);

            // wake up consumers
            pthread_cond_broadcast(&cond_cons);
        }

        pthread_cond_wait(&cond_cons, &mutex);
        
        // // all producers are created, tell consumers this info
        // if (count_prod == NB_Producers) {
        //     flag_prod = 1;
        //     pthread_cond_broadcast(&cond_prod);
        // }
        // // wait till all consumers are created
        // while (flag_cons != 1) {
        //     pthread_cond_wait(&cond_cons, &mutex);
        // }

        // // write 
        // random_value = rand() % 100 + 1;
        // printf("producer: %d\t push value: %d\n", *(int*)arg, random_value);
        // stack[0] = random_value;
        // print_stack(stack);
        // sleep(1);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit((void*) 0);
}

void* thread_pop(void* arg) {
    int i;
    count_cons++;

    while (1) {
        pthread_mutex_lock(&mutex);
        // // all consumers are created, tell producers
        // if (count_cons == NB_Consumers) {
        //     flag_cons = 1;
        //     pthread_cond_broadcast(&cond_cons);
        // }
        // // wait till all producers are created
        // while (flag_prod != 1) {
        //     pthread_cond_wait(&cond_prod, &mutex);
        // }
        // read
        for (i = NB_INTEGERS - 1; i > -1; i --) {
            if (stack[i] != 0) {
                printf("consumer: %d\t pop value: %d\n", *(int*)arg, stack[i]);
                stack[i] = 0;
                print_stack(stack);
                //
                pthread_cond_broadcast(&cond_prod);
                break;
            }
        }
        if (i == -1) {
            // no value availabe
            // tell the producer to produce
            pthread_cond_broadcast(&cond_prod);
            pthread_cond_wait(&cond_cons, &mutex);
        } else {
            // consumed, exit
            // pthread_mutex_unlock(&mutex);
            // break;
        }
        // if (stack[0] != 0) {
        //     printf("consumer: %d\t pop value: %d\n", *(int*)arg, stack[0]);
        //     for (i = 0; i < NB_INTEGERS-1; i++) {
        //         if (stack[i] == 0) {
        //             break;
        //         }
        //         stack[i] = stack[i+1];
        //     }
        // }

        sleep(1);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit((void*) 0);
}

void print_stack(int stack[]) {
    printf("    Stack: [");
    int i;
    for (i = 0; i < NB_Producers; i++) {
        printf("%d, ", stack[i]);
    }
    if (i == 0) {
        printf("NULL");
    }
    printf("\b\b]\n");
}


// need a semaphore?? queue?

// producer: wait after produce?