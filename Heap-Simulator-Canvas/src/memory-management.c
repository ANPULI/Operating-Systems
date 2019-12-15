#include <os-memory.h>


void first_fit(int size, freezone* fz) {
    int found = freelist;
    int previous = -1;

    while ((found != -1) && (heap[found] < size)) {
        previous = found;
        found = heap[found + 1];
    }

    fz->previous = previous;
    fz->found = found;
}

void best_fit(int size, freezone* fz) {
    // TODO
    int found = freelist;
    int previous = -1;
    int i = 0;
    freezone fzs[HEAP_SIZE/MIN_BLOCK_SIZE];

    // printf("break point 1\n");
    while (found != -1) {
        if (heap[found] >= size) {
            /* same pointer!! big mistake*/
            // fz->previous = previous;
            // fz->found = found;
            // fzs[i] = fz;
            fzs[i].previous = previous;
            fzs[i].found = found;
            i++;
        }
        previous = found;
        found = heap[found+1];
    }
    // printf("break point 2\n");

    printf("i = %d\n", i);
    if (i == 0) {
        fz->previous = previous;
        fz->found = found;
    } else {
        *fz = fzs[--i];
        i--;
        printf("i = %d\n", i);

        while (i >= 0) {
            printf("%d\t%d\n", fz->found, fzs[i].found);
            if (heap[fz->found] >= heap[fzs[i].found]) {
                *fz = fzs[i];
            }
            i--;
        }
    }
    printf("i = %d\n", i);
    // printf("break point 3\n");
}

void worst_fit(int size, freezone* fz) {
    // TODO
    int found = freelist;
    int previous = -1;
    int i = 0;
    freezone fzs[HEAP_SIZE/MIN_BLOCK_SIZE];

    // printf("break point 1\n");
    while (found != -1) {
        if (heap[found] >= size) {
            fzs[i].previous = previous;
            fzs[i].found = found;
            i++;
        }
        previous = found;
        found = heap[found+1];
    }
    // printf("break point 2\n");

    printf("i = %d\n", i);
    if (i == 0) {
        fz->previous = previous;
        fz->found = found;
    } else {
        *fz = fzs[--i];
        i--;
        printf("i = %d\n", i);

        while (i >= 0) {
            printf("%d\t%d\n", fz->found, fzs[i].found);
            if (heap[fz->found] <= heap[fzs[i].found]) {
                *fz = fzs[i];
            }
            i--;
        }
    }
    printf("i = %d\n", i);
    // printf("break point 3\n");
}


void* heap_malloc(int size) {
    freezone result;                    //free zone found for the allocation
    int allocation_size = size + 1;     //size of the allocated zone
    void *ptr = NULL;                   //pointer to the allocated zone
    
    find_free_zone(size, &result);
    
    //Cannot find a free zone
    if (result.found == -1)
        return NULL;
    
    // TODO
    if (heap[result.found] == HEAP_SIZE - 1) {
        // check if the whole heap is empty as we cannot modify simulator...
        // can be improved by modifying init_heap()
        nb_block = 0;
    }

    printf("found: %d\t previous: %d\n", result.found, result.previous);

    int idx = result.found, prev = result.previous;
    // heap[idx] = size;   // DO IT LATER!!!

    // not occupy all remaining space
    if (size <= heap[idx] - MIN_BLOCK_SIZE) {
        // set the next free zone
        int next = idx + size + 1;
        heap[next] = heap[idx] - size - 1;
        heap[next+1] = heap[idx+1];
        heap[idx+1] = next;
    }

    if (prev == -1) {
        // freelist is idx
        freelist = heap[idx+1];
    } else {
        heap[prev+1] = heap[idx+1];
    }
    
    heap[idx] = size;                       // set the size 
    printf("Setting the size: %d\n", size);


    ptr = &heap[idx+1];   // set pointer
    // add_allocation(ptr);
    // print_allocations();
    return (void*) ptr;
}


int heap_free(void *dz) {
    // TODO

    

    // simply set the current area as free, does not consider previous/next
    //  ----sp0000----    s:size, p:next
    
    // clean the zone
    int idx = ptr2ind(dz) - 1;
    // printf("idx: %d\n", idx);
    int size = heap[idx];
    int i;
    // clean up 
    // for (i = idx + 1; i < idx + size + 1; i++) {
    //     heap[i] = 0;
    // }

    // remove allocation
    // remove_allocation((char*) dz);
    // print_allocations();

    // update freelist

    // idx is the new freelist
    // 1. idx left to freelist OR 2. there is no freelist
    if ( (idx < freelist) || (freelist == -1) ) {
        heap[idx+1] = freelist;
        freelist = idx;
        // concat freezones
        concat_freezone(idx);
        return 0;
    } 
    // printf("should not go beyond this\n");


    // idx is not the new freelist (freelist left to idx)
    // 1. if no freezone except freelist
    i = freelist;
    if (heap[i+1] == -1) {
        heap[i+1] = idx;
        heap[idx+1] = -1;
        // concat freezones
        concat_freezone(i);  // freelist actually
        return 0;
    } 

    // 2. there is a freezone besides freelist
    // use freelist to find next free zone
    // loop through the heap
    int j = 1, prev, concat_list[HEAP_SIZE/MIN_BLOCK_SIZE];
    concat_list[0] = i;
    while (heap[i+1] != -1) {
        concat_list[j] = heap[i+1];
        if (heap[i+1] > idx) {
            break;
        }
        j ++;
        i = heap[i+1];
    }
    // print_concat_list(concat_list, j);
    if (heap[i+1] == -1) {
        // no free zone after idx               // idx is the last
        j--;
        
        prev = concat_list[j];
        heap[prev+1] = idx;

        heap[idx+1] = -1;
        while (j >= 0) {
            concat_freezone(concat_list[j]);
            j--;
        }
        // concat_freezone(i);
    } else {
                                                // zone idx zone
        heap[idx+1] = heap[i+1];
        prev = concat_list[j-1];
        heap[prev+1] = idx;

        printf("heap[idx+1]: %d\n", heap[idx+1]);
        j++;
        concat_list[j] = concat_list[j-1];
        concat_list[j-1] = idx;
        // print_concat_list(concat_list, j);
        printf("the next @22: %d\n", heap[22]);
        while (j >= 0) {
            concat_freezone(concat_list[j]);
            j--;
        }
        // concat_freezone(idx);  // right
        // concat_freezone(j);  // left
    }
    // concat_freezone(freelist);
    return 0;
}

int heap_defrag() {
    // TODO
    // i: iterator of allocaltions
    // j: iterator of the heap, will be freelist at last
    printf("===START DEFRAG===\n");

    int i, j = 0, k, offset, size;
    for (i = 0; i < nb_block; i++) {
        offset = ((int) ((void*)*allocations[i] - (void*)&heap[j])) - 1;
        size = heap[ptr2ind(*allocations[i])-1];
        printf("size: %d\t offest: %d\n", size, offset);
        printf("before: \t *allocations[%d]: %d\n", i, *allocations[i]);
        printf("allocations[%d]: %d\n", i, allocations[i]);
        *(allocations[i]) = &heap[j+1];
        printf("after: \t *allocations[%d]: %d\n", i, *allocations[i]);
        printf("allocations[%d]: %d\n", i, allocations[i]);
        // printf("check\n");

        for (k = 0; k < size + 1; k++, j++) {
            printf("heap[j+offset]: %d\n",heap[j+offset]);
            heap[j] = heap[j+offset];
            // printf("%d\n", j);
        }
        // printf("check\n");

    }
    heap[j] = HEAP_SIZE - j - 1;
    heap[j+1] = -1;
    freelist = j;
    return heap[j];
}


void concat_freezone(int i) {
    int size = heap[i];
    int next = heap[i+1];
    if (next == i + size + 1) {
        heap[i+1] = heap[next+1];
        heap[i] += (heap[next]+1);
    }
}

void print_concat_list(int concat_list[], int j) {
    while (j >= 0) {
        printf("concat list[%d]: %d\n", j, concat_list[j]);
        j--;
    }
}

void add_allocation(char** ptr) {
    printf("---add allocation---\n");
    int i = 0, j;
    while ((i < nb_block) && (*ptr > *allocations[i])) {
        i++;
    }

    for (j = nb_block; j > i; j--) {
        // printf("allocations[j-1=%d]: %d\n", j-1, *allocations[j-1]);
        allocations[j] = allocations[j-1];
        // printf("allocations[j=%d]: %d\n", j, *allocations[j]);
    }
    allocations[i] = ptr;
    // printf("ptr: %d \t *allocations[0]: %d\n", ptr, *allocations[0]);
    // printf("allocations[%d]: %d\n", i, *allocations[i]);

    nb_block++;
}

void remove_allocation(char** ptr) {
    int i;
    for (i = 0; i < nb_block; i++) {
        // printf("%d\t %d\n", ptr, *allocations[i]);
        // printf("%d\t %d\n", ptr2ind(ptr), ptr2ind(*allocations[i]));
        if (*ptr == *allocations[i]) {
            printf("ptr: %d\n", ptr2ind(*ptr));
            break;
        }
    }
    while (i < nb_block - 1) {
        allocations[i] = allocations[i+1];
        i++;
    }
    nb_block--;
}

void print_allocations() {
    printf("-----print allocations-----\n");
    int i;
    for (i = 0; i < nb_block; i++) {
        // printf("pointer %d @ %d \t", i, ptr2ind(*allocations[i]));
        printf("%d: %d\t", i, *allocations[i]);
        
    }
    printf("\n");
}