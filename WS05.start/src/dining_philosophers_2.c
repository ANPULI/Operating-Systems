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
#include <errno.h>


#define N 5
#define USING 2
#define TAKEN 0
#define EMPTY 1
#define EATING 5
#define READY 4
#define SEMI 3
#define HUNGRY 2
#define IDLE 0
#define IS_SHARED 1


typedef struct{
    int vals[N];
} chopsticks;
chopsticks* chops;

char mutex[] = "mutex";
sem_t * mx;

int status;
void think(int i);
void eat(int i);
void stop_eat(int i);
void display_chops();
char* determine_status(int status);

int main(int argc, char** argv) {
    
    // INIT
    status = IDLE;
    int i, left, right, fd;
    if ((fd = shm_open("chops", O_CREAT|O_RDWR, 0666)) == -1) {perror("fd fail");}
    ftruncate(fd, sizeof(chopsticks));
    chops = (chopsticks*)mmap(0, sizeof(chopsticks), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    for (i = 0; i < N; i++) {
        chops->vals[i] = EMPTY;
    }
    
    printf("Init mutex\n");
    // sem_init(mx, IS_SHARED, 1);
    if ((mx = sem_open(mutex, O_CREAT|O_EXCL|O_RDWR, 0666, 1)) == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            exit(1);
        }
        perror("sem_mx_already_exist");
        // exit(1);
        sem_close(mx); sem_unlink(mutex);
        printf("make new mx\n");
        mx = sem_open(mutex, O_CREAT|O_EXCL|O_RDWR, 0666, 1);
    }

    // Create N child processes
    printf("Create N child processes\n");
    for (i = 0; (i < N) && (fork() > 0); i++) {}

    if (N == i) {
        // parent
        for (i = 0; i < N; i++) {
            wait(NULL);
        }
        sem_close(mx);
        sem_unlink(mutex);
        return 0;
    }

    while (1) {
        think(i);
        // take one at a time
        sem_wait(mx);  // P(Mx)
        left = chops->vals[i % N];
        right = chops->vals[(i+1) % N];
        switch (status) {
            case HUNGRY:
                // take left
                if ((left == EMPTY) && (right == EMPTY)) {
                    chops->vals[i % N] = TAKEN;
                    status = SEMI;
                }
                break;
            
            case SEMI:
                // take right
                if (right == EMPTY) {
                    chops->vals[(i+1) % N] = TAKEN;
                    status = READY;
                }
                break;
            default:
                perror("status should not appear here");
                break;
        }
        sem_post(mx);

        if (READY == status) {
            eat(i);
            display_chops();
        }
        if (EATING == status) {
            sem_wait(mx); // P(Mx)
            stop_eat(i);
            sem_post(mx); // V(Mx)
        }
    }

    return 0;
}

void think(int i) {
    printf("philosopher %d\t status: %s\n", i, "THINKING");  // note we don't change the real status here as we want to keep the original status
    sleep(1);
    if (SEMI != status) {
        status = HUNGRY;
    }
    printf("philosopher %d\t status: %s\n", i, determine_status(status));
}

void eat(int i) {
    status = EATING;
    printf("philosopher %d\t status: %s\n", i, determine_status(status));
    // change chops status for better visualization
    sem_wait(mx); // P(Mx)
    chops->vals[i % N] = USING;
    chops->vals[(i+1) % N] = USING;
    sem_post(mx); // V(Mx)
    sleep(1);
}

void stop_eat(int i) {
    chops->vals[i % N] = EMPTY;
    chops->vals[(i+1) % N] = EMPTY;
    status = IDLE;
    printf("philosopher %d\t status: %s\n", i, determine_status(status));
}

void display_chops() {
    int j;
    for (j = 0; j < N; j++) {
        printf("%d ", chops->vals[j]);
    }
    printf("\n");
}

char* determine_status(int status) {
    switch (status)
    {
    case EATING:
        return "EATING";
    case READY:
        return "READY";
    case SEMI:
        return "SEMI";
    case HUNGRY:
        return "HUNGRY";
    case IDLE:
        return "IDLE";
    default:
        return "ERROR";
    }
}