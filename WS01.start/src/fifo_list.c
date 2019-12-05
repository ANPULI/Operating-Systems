#include <fifo.h>
#include <list.h>
#include <stdlib.h>

struct list_type* l;
cell* c;


/* Initialization of the queue */
int init_queue() {
    l = malloc(sizeof(struct list_type));
    init_list(l);
    return 0;  // normal
}

/* Insertion of a new element.
 The new element is added to the head of the queue. */
int queue(void* element) {
    insert_head(l, element);
    return 0;
}

/* Extraction of the element at the tail of the queue.
 Returns (also deletes) that element.  */
void* dequeue() {
    c = extract_tail(l);
    return c->content;
}

/* Returns the number of elements in the queue */
int size() {
    return list_size(l);
}