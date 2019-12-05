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

void set_semaphore_server(char* s);
void set_semaphore_client(char* c);
void set_mutex();
sem_t *s_client, *s_server, *mutex;
char s_server_id[32], s_client_id[32];

void sig_hand(int sig);
void sig_hand_SIGUSR1(int sig);

static int run = 1;
char server_id[32], client_id[32];

pid_t c_pid[NB_CONVERTERS];

int main(int argc, char** argv) {

    /*
        argc: 2
        argv: <conversion_server> <server_id> 
    */
    
    int i, fd_server, fd_client;
    float src_amount;
    char src_currency[4], target_currency[4];
    char suffix[] = "_shm";



    // alter SIGNIT behavior
    sigset_t sig_proc;
    struct sigaction action;
    sigemptyset(&sig_proc);
        action.sa_mask = sig_proc;
        action.sa_flags = 0;
        action.sa_handler = sig_hand;
    sigaction(SIGINT, &action, 0);
    // alter SIGUSR1 behavior
        action.sa_handler = sig_hand_SIGUSR1;
    sigaction(SIGUSR1, &action, 0);

    // setting server_id_shm, tedious but required step
    strcpy(server_id, argv[1]);
    strcat(server_id, suffix);

    // setting server semaphore
    set_semaphore_server(server_id);
    
    // setting mutex
    set_mutex();

    // creating shared memory of server
    fd_server = shm_open(server_id, O_CREAT|O_RDWR, 0666);
    ftruncate(fd_server, sizeof(server_mem));
    server_memory = (server_mem*)mmap(0, sizeof(server_mem), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);

    // create child processes
    for (i = 0; (i < NB_CONVERTERS) && ((c_pid[i] = fork()) > 0); i++){}

    // main func
    while (run){
        // wait for client to release signal
        if (NB_CONVERTERS == i) {
            // parent
            sem_wait(s_server);
        } else {
            // child, suspend for SIGUSR1
            sigfillset(&sig_proc);
            sigdelset(&sig_proc, SIGUSR1);
            sigsuspend(&sig_proc);

            // do conversions
            strcpy(client_id, server_memory->client_id);  // client_id = server_memory->client_id;
            fd_client = shm_open(client_id, O_RDWR, 0666);
            client_memory = (client_mem*)mmap(0, sizeof(client_mem), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);
            
            strcpy(src_currency, server_memory->currency);
            src_amount = server_memory->amount;
            strcpy(target_currency, determine_currency(i));
            
            client_memory->vals[i] = convert(src_currency, target_currency, src_amount);
            // shm_unlink(client_id);
            continue;
        }

        // now we have client requests
        // connect shared memory of client
        strcpy(client_id, server_memory->client_id);  // client_id = server_memory->client_id;
        fd_client = shm_open(client_id, O_RDWR, 0666);
        client_memory = (client_mem*)mmap(0, sizeof(client_mem), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);

        // set client semaphore
        printf("set semaphore for %s\n", client_id);
        set_semaphore_client(client_id);

        // convert rates and write
        strcpy(src_currency, server_memory->currency);  // src_currency = server_memory->currency;
        src_amount = server_memory->amount;
        printf("Doing conversion for %s for: %s %.3f\n", client_id, src_currency, src_amount);
        for (i = 0; i < NB_CONVERTERS; i++) {
            kill(c_pid[i], SIGUSR1);
            /*
            if (fork() == 0) {
                strcpy(target_currency, determine_currency(i));
                // send result
                client_memory->vals[i] = convert(src_currency, target_currency, src_amount);
                // display_result(i, convert(src_currency, target_currency, src_amount));
                exit(1);
            }
            */
        }

        // wait child processes
        // for (i = 0; i < NB_CONVERTERS; i++) {wait(NULL);}
        
        // release client barrier
        sem_post(s_client);

        // close client semaphore
        sem_close(s_client);
        // no need to destroy, client will do it
    }

    // unlink everything
    printf("unlinking\n");
    shm_unlink(client_id);
    shm_unlink(server_id);
    munmap(server_memory, sizeof(server_mem));
    sem_close(s_server);
    sem_unlink(s_server_id);
    sem_close(mutex);
    sem_unlink("mutex");

    return 0;
}

void set_semaphore_server(char* s){
    strcpy(s_server_id, "s_");
    strcat(s_server_id, s);
    if ((s_server = sem_open(s_server_id, O_CREAT|O_EXCL|O_RDWR, 0666, 0)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            exit(1);
        }
        // semaphore already created, shouldn't be in this case also
        perror("sem_server_already_exist");
        // exit(1);
        // printf("s_server_exist\n");
        sem_close(s_server);sem_unlink(s_server_id);
        s_server = sem_open(s_server_id, O_CREAT|O_EXCL|O_RDWR, 0666, 0);
    }
}

void set_semaphore_client(char* c) {
    strcpy(s_client_id, "s_");
    strcat(s_client_id, c);
    if ((s_client = sem_open(s_client_id, O_EXCL|O_RDWR)) == SEM_FAILED) {
        if (errno != ENOENT) {
            perror("sem_open");
            exit(1);
        }
        // semaphore not created, shouldn't be in this case also
        perror("sem_client_not_exist");
        exit(1);
    }
}

void set_mutex() {
    if ((mutex = sem_open("mutex", O_CREAT|O_EXCL|O_RDWR, 0666, 1)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open_mutex");
            exit(1);
        }
        // semaphore already created, shouldn't be in this case also
        perror("sem_mutex_already_exist");
        sem_close(mutex); sem_unlink("mutex");
        mutex = sem_open("mutex", O_CREAT|O_EXCL|O_RDWR, 0666, 1);
    }
}

void sig_hand(int sig) {
    printf("SIGINT received. Close program.\n");
    run = 0;
    printf("unlinking\n");
    shm_unlink(client_id);
    shm_unlink(server_id);
    munmap(server_memory, sizeof(server_mem));
    sem_close(s_server);
    sem_unlink(s_server_id);
    sem_close(mutex);
    sem_unlink("mutex");

    // kill child processes
    int i;
    for (i = 0; i < NB_CONVERTERS; i++) {
        kill(c_pid[i], SIGKILL);
    }
    exit(1);
}

void sig_hand_SIGUSR1(int sig) {
    printf("SIGUSR1 received. Start conversion\n");

}