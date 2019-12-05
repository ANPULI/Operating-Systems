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
}

void worst_fit(int size, freezone* fz) {
    // TODO
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
    int idx = result.found, prev = result.previous;
    // heap[idx] = size;   // DO IT LATER!!!

    // not occupy all remaining space
    if (size <= heap[idx] - 2) {
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


    ptr = (void *) (&heap[idx+1]);   // set pointer

    return ptr;
}


int heap_free(void *dz) {
    // TODO

    // simply set the current area as free, does not consider previous/next
    //  ----sp0000----    s:size, p:next
    
    // clean the zone
    int idx = ptr2ind(dz) - 1;
    printf("idx: %d\n", idx);
    int size = heap[idx];
    int i;
    // clean up 
    // for (i = idx + 1; i < idx + size + 1; i++) {
    //     heap[i] = 0;
    // }


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

    // 2. there is a freelist besides freelist
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
    print_concat_list(concat_list, j);
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
        print_concat_list(concat_list, j);
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
    return 0;
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