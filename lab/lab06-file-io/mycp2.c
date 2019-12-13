/*
Question 2: Custom file copy
Write a program mycp that takes two filenames as parameters:
$ ./mycp <file1> <file2>
This program completely copies the contents of the file file1 into the file file2.
There are 2 conditions required for this program to work without returning an error:
1. the file file1 must exist and be a regular file,
2. The file file2 must not exist.  

*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define BUFSZ 1024

int main (int argc, char** argv) {
    int fd1, fd2, rdsz=0;
    struct stat st;
    char buffer[BUFSZ];

    // check that file1 does exist
    if (stat(argv[1], &st) != 0) {
        perror("File1 does not exist.");
        exit(1);
    }
    if (!S_ISREG(st.st_mode)) {
        perror("File1 is not a reg file.");
        exit(1);
    }
    // check that file2 does not exist
    if (access(argv[2], F_OK) == 0) {
        perror("File2 exists.");
    }

    // open file1 and file2
    if ((fd1 = open(argv[1], O_RDONLY)) == -1)
        return EXIT_FAILURE;
    if ((fd2 = open(argv[2], O_WRONLY|O_CREAT)) == -1)
        return EXIT_FAILURE;

    // copy everything from file1 to file2
    while ( (rdsz = read(fd1, buffer, BUFSZ)) > 0) {
        write(fd2, buffer, rdsz);
    }
    
    return EXIT_SUCCESS;
}
