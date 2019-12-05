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
#include "converters.h"



typedef struct {
    char client_id[32];
    char currency[4];
    float amount;
} server_mem;

server_mem *server_memory;

typedef struct {
    float vals[NB_CONVERTERS];    
} client_mem;

client_mem *client_memory;
// float client_memory[NB_CONVERTERS];  //{EUR, GBP, USD, JPY, CNY}

void set_semaphores(char* s, char* c);
void set_mutex();
sem_t *s_client, *s_server, *mutex;
char s_server_id[32], s_client_id[32];

int main(int argc, char** argv) {
    /*
        argc: 5
        argv: <conversion_client> <server_id> <client_id> <currency> <amount>
    */

    int i, fd_server, fd_client;
    float src_amount;
    char src_currency[4];
    char server_id[32], client_id[32], suffix[] = "_shm";

    // setting server_id_shm and client_id_shm, tedious but required step
    strcpy(server_id, argv[1]);
    strcat(server_id, suffix);
    strcpy(client_id, argv[2]);
    strcat(client_id, suffix);
    // set amount and currency
    strcpy(src_currency, argv[3]);
    src_amount = atof(argv[4]);

    // setting semaphores
    set_semaphores(server_id, client_id);

    // setting mutex
    set_mutex();

    // create shared memory of client
    fd_client = shm_open(client_id, O_CREAT|O_RDWR, 0666);
    ftruncate(fd_client, sizeof(client_mem));
    client_memory = (client_mem*)mmap(0, sizeof(client_mem), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);
    for (i = 0; i < NB_CONVERTERS; i++) {
        client_memory->vals[i] = 0.0;
    }

    // open shared memory of server
    printf("server_id: %s\n", server_id);
    if ((fd_server = shm_open(server_id, O_RDWR, 0666)) == -1) {perror("fd_server fail");}
    ftruncate(fd_server, sizeof(server_mem));
    server_memory = (server_mem*)mmap(0, sizeof(server_mem), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);

    // put info in server memory
    // printf("%s %s %.3f\n", server_memory->client_id, server_memory->currency, server_memory->amount);
    // printf("Giving server my information\n");
    strcpy(server_memory->client_id, client_id);
    strcpy(server_memory->currency, src_currency);
    server_memory->amount = src_amount;
    // printf("%s %s %.3f\n", server_memory->client_id, server_memory->currency, server_memory->amount);

    // release server barrier
    // printf("sem_post(s_server)\n");
    sem_post(s_server);
    
    // request 
    // printf("sem_wait(s_client)\n");
    sem_wait(s_client);
    
    // printf("%s\n",argv[2]);
    // display result
    printf("Conversion of %s for: %s %.3f\n", argv[2], src_currency, src_amount);
    for (i = 0; i < NB_CONVERTERS; i++) {
        display_result(i, client_memory->vals[i]);
    }

    // unlink everything and destroy client semaphore
    shm_unlink(client_id);
    /*
        IMPORTANT CAVEAT!
        shm_unlink is not based on each process,
        but cuts the connection of this piece wholly
    */
    // shm_unlink(server_id);   
    munmap(client_memory, sizeof(client_mem));
    sem_close(s_client);
    sem_close(s_server);
    sem_unlink(s_client_id);

    return 0;
}

void set_semaphores(char* s, char* c) {
    strcpy(s_server_id, "s_");
    strcat(s_server_id, s);
    strcpy(s_client_id, "s_");
    strcat(s_client_id, c);
    if ((s_server = sem_open(s_server_id, O_EXCL|O_RDWR)) == SEM_FAILED) {
        if (errno != ENOENT) {
            perror("sem_open");
            exit(1);
        }
        // semaphore not created, shouldn't be in this case also
        perror("sem_server_not_exist");
        exit(1);
    }
    
    if ((s_client = sem_open(s_client_id, O_CREAT|O_EXCL|O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            exit(1);
        }
        // semaphore already created, shouldn't be in this case also
        perror("sem_client_already_exist");
        // exit(1);
        // printf("s_client_exist\n");
        sem_close(s_client);sem_unlink(s_client_id);
        printf("make new s_client\n");
        s_client = sem_open(s_client_id, O_CREAT|O_EXCL|O_RDWR, 0666, 0);
        if (s_client == SEM_FAILED) {
            printf("failed\n");
            if (errno == EEXIST) {
                printf("still exist\n");
            }
        }
    }
}

void set_mutex() {
    if ((mutex = sem_open("mutex", O_RDWR, 0666, 1)) == SEM_FAILED) {
        if (errno != ENOENT) {
            perror("sem_open_mutex");
            exit(1);
        }
        perror("sem_mutex_not_exist");
        exit(1);
    }
}