#include <os-memory.h>
#include <string.h>
#include <ctype.h>


void init_heap() {
    memset(heap, 0, HEAP_SIZE);
    heap[0] = HEAP_SIZE - 1;
    heap[1] = -1;
    freelist = 0;
}


void print_heap() {
    int LINESZ = 16;
    int i, j;
    
    printf("======== HEAP (FREELIST @%d) ========\n", freelist);
    for (i = 0; i < (HEAP_SIZE / LINESZ); i++) {
        printf("\033[0;36m");
        for (j = 0; j < LINESZ; j++) {
            printf("\t%3d", j + LINESZ*i);
        }
        printf("\n");
        printf("\033[0;34m");
        for (j = 0; j < LINESZ; j++) {
            printf("\t%3d", heap[j + LINESZ*i]);
        }
        printf("\n");
        printf("\033[0m");
        for (j = 0; j < LINESZ; j++) {
            if (isalnum(heap[j + LINESZ*i])) {
                printf("\t%3c", heap[j + LINESZ*i]);
            } else {
                printf("\t ");
            }
        }
        printf("\n");
    }
    printf("====================================\n\n");
}



void find_free_zone(int size, freezone* fz) {
    switch (ALLOCATION_STRATEGY) {
        case FIRSTFIT:
            first_fit(size, fz);
            break;
        case BESTFIT:
            best_fit(size, fz);
            break;
        case WORSTFIT:
            worst_fit(size, fz);
            break;
        default:
            perror("Undefined allocation strategy");
            exit(1);
    }
}



int main(int argc, char** argv) {
    
    init_heap();
    
    print_heap();
    
    run_simulation();
    
    return EXIT_SUCCESS;
}




