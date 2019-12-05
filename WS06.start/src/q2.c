/*
Question 2 - Multi-threaded file conversion - Fixed pool of threads

    Now consider that the main thread only creates a fixed number NB_THREAD of secondary threads at startup. 
    NB_THREAD can be smaller than the number of files to process. 
    Immediately after converting a file, a thread must check whether there still are files to process. 
    If such is the case, the thread takes on the conversion of one of the remaining files; otherwise it ends.

*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define NB_THREAD 3

char** files;
int count;
int NB_FILE;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * upper(void* arg);

int main (int argc, char ** argv) {
    NB_FILE = argc - 1;
    int i, status, *pt_i;
    pthread_t tid[NB_THREAD];
    // pthread_t tid[NB_FILE];

    // files need to be global
    files = (char**) malloc(NB_FILE*sizeof(char *));
    for (i = 0; i < NB_FILE; i++) {
        files[i] = argv[1+i];
    }
    count = 0;

    // for (i = 0; i < NB_FILE; i++) {
    for (i = 0; i < NB_THREAD; i++) {
        pt_i = (int *) malloc(sizeof(int));
        *pt_i = i;
        if (pthread_create(&(tid[i]), NULL, upper, (void *) pt_i) != 0) {
            perror("create fail");
        } else {
            printf("create success, i: %d\n", i);
        }
    }

    for (i = 0; i < NB_THREAD; i++) {
        if (pthread_join(tid[i], (void**) status) != 0) {
            perror("wait fail");
        }
    }

    return EXIT_SUCCESS;
}

void *upper(void *arg) {
    FILE *fp_src, *fp_dest;
    char dest_fname[128];
    int c = 1;
    int i = * (int *) arg;

    while (1) {
        // fetch the file to capitalize, set lock
        pthread_mutex_lock(&mutex);
        if (count == NB_FILE) {
            pthread_mutex_unlock(&mutex);
            pthread_exit((void*) 0);
        }
        char* file = files[count];
        printf("converting file: %s\t by %d\n", file, i);
        count++;
        pthread_mutex_unlock(&mutex);
        // fetch complete, release lock

        // capitalize
        fp_src = fopen(file, "r");
        strcpy(dest_fname, file);
        strcat(dest_fname, ".UPPER.txt");
        printf("%s\n", dest_fname);
        fp_dest = fopen(dest_fname, "w");
        if (fp_src == NULL || fp_dest == NULL) {
            perror("fopen");
            exit(1);
        }

        while ((c = fgetc(fp_src)) != EOF) {
            fputc(toupper(c), fp_dest);
        }
        fclose(fp_src);
        fclose(fp_dest);
    }

    return NULL;
}