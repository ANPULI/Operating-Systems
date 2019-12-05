/*
Question 1 - Multi-threaded file conversion - Unlimited number of threads

    Modify the program to accept the names of several files as argument. 
    The program must create a secondary thread for each filename, 
    and each secondary thread performs the conversion on the corresponding text file.
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>


void * upper(void* arg);

int main (int argc, char ** argv) {
    // FILE *fp_src, *fp2;
    // char dest_fname[128];
    const int NB_FILE = argc - 1;
    int i, status;
    pthread_t tid[NB_FILE];

    for (i = 0; i < NB_FILE; i++) {
        // fp_src = fopen(argv[1+i], "r");
        if (pthread_create(&(tid[i]), NULL, upper, (void *) argv[1+i]) != 0) {
            perror("create fail");
        } else {
            printf("create success, i: %d\n", i);
        }
    }

    for (i = 0; i < NB_FILE; i++) {
        if (pthread_join(tid[i], (void**) status) != 0) {
            perror("wait fail");
        }
    }

//     fp1 = fopen (argv[1], "r");
//     strcpy(dest_fname, argv[1]);
//     strcat(dest_fname, ".UPPER.txt");
//     printf("%s\n", dest_fname);
//     fp2 = fopen (dest_fname, "w");
//     if ((fp1 == NULL) || (fp2 == NULL)) {
//         perror ( "fopen");
//         exit (1);
//     }
    
//     while (c != EOF) {
//         c = fgetc(fp1);
//         if (c != EOF)
//             fputc(toupper(c), fp2);
// //            fputc(toupper(c), stdout);
//     }
    
//     fclose (fp1);
//     fclose (fp2);
    
    // sleep(10);

    return EXIT_SUCCESS;
}

void *upper(void *arg) {
    FILE *fp_src, *fp_dest;
    char dest_fname[128];
    int c = 1;

    fp_src = fopen((char*) arg, "r");
    strcpy(dest_fname, (char*) arg);
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

    return NULL;
}