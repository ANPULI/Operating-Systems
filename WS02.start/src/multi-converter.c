/**** multi-converter.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <converters.h>


#define _XOPEN_SOURCE 700



int main(int argc, char **argv)
{
    
    /**** TODO ****/
    int i;
    char src_currency[4], target_currency[4];
    // char* targets[5] = {"EUR", "GBP", "USD", "JPY", "CNY"};
    float src_amount;

    strcpy(src_currency, argv[1]);
    src_amount = atof(argv[2]);
    
    printf("Conversion for: %s %.3f\n", src_currency, src_amount);
    for (i = 0; i <5; i++) {
        if (fork() == 0) {
            // strcpy(target_currency, targets[i]);
            strcpy(target_currency, determine_currency(i));
            display_result(i, convert(src_currency, target_currency, src_amount));
            exit(1);
        }
    }

    for (i = 0; i < 5; i++)
        wait(NULL);
    printf("End of conversion\n");
    return EXIT_SUCCESS;
}
