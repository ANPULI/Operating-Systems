#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define HEAP_SIZE 128
#define MIN_BLOCK_SIZE 2

/** Memory allocation strategies **/
#define FIRSTFIT 0
#define BESTFIT 1
#define WORSTFIT 2
/** default strategy is first fit */
#ifndef ALLOCATION_STRATEGY
#define ALLOCATION_STRATEGY FIRSTFIT
#endif


typedef struct {
    int found;      //index in the heap array of the first cell of the target free zone
    int previous;   //index of the first cell of the free zone that precedes the target
} freezone;

char heap[HEAP_SIZE];                           /* Array representation of the heap */
int freelist;                                   /* List of free memory zones */
char *allocations[HEAP_SIZE/MIN_BLOCK_SIZE];    /* List of allocated memory zones */

/* ptr2ind translates the address of a pointer into its index value in the heap array */
#define ptr2ind(p) (int) ((void*)p - (void*)heap)

/**** Internal simulation calls ****/
//void init_heap();
void print_heap();
void find_free_zone(int size, freezone* fz);
void run_simulation();


/**** TODO - memory management routines ****/


/* allocates a free memory zone whose size is given as a parameter */
/* returns the pointer to the beginning of the writable area of the allocated zone */
/* returns NULL if there is no free zone big enough for the allocation */
void* heap_malloc(int size);

/* releases a memory zone given as a parameter (pointer to the beginning of the writable area) */
/* returns 0 if the deallocation is successful, -1 otherwise */
int heap_free(void *dz);

/* repositions all allocated memory zones to contiguous areas */
/* returns the size of the unique free zone that results from the defragmentation */
int heap_defrag();

/* returns the index of the first free memory zone big enough for the requested size */
/* returns -1 if no free zone is big enough */
void first_fit(int size, freezone* fz);

/* returns the index of the smallest free memory zone big enough for the requested size */
/* returns -1 if no free zone is big enough */
void best_fit(int size, freezone* fz);

/* returns the index of the biggest free memory zone if it is big enough for the requested size */
/* returns -1 if no free zone is big enough */
void worst_fit(int size, freezone* fz);

