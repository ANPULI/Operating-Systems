#include <stack.h>
#include <list.h>
#include <stdlib.h>
#include <stdio.h>

// #include "list_impl.c"

/* Stack structure */
struct list_type* l;

cell* c;

int init_stack() {
    // fprintf(stderr,"catch Segmentation fault!!!\n");
    // printf("sizeof(struct list_type*) = %d\n", sizeof(struct list_type*));
    // printf("sizeof(struct list_type) = %d\n", sizeof(struct list_type));
    l = malloc(sizeof(struct list_type));
    init_list(l);
    // fprintf(stderr,"catch Segmentation fault!!!\n");
    return size();
}

int push(void* element) {
    insert_head(l, element);
    return size();
}

void* pop() {
    // fprintf(stderr,"catch Segmentation fault!!!\n");
    c = extract_head(l);
    // fprintf(stderr,"catch Segmentation fault!!!\n");
    return (char*) c->content;
}

int size() {
    return list_size(l);
}

// int main() {
//     char* mywords[4] = {"The Force", "is", "strong", "within this Jedi"};
// 	int i;
// 	int nb;
	
// 	// struct list_type *l;
//     l = malloc(sizeof(struct list_type*));
//     struct cell_type *c;
//     // init_list(l);
//     init_stack();

//     printf("starting\n");
	
// 	for(i = 0; i < 4; i++)
// 		insert_head(l, mywords[i]);
//     // printf(l->head->content);

// 	nb = list_size(l);
//     printf("size: %d\n", nb);

//     printf("head: %s\n", (char *)l->head->content);

// 	for(i = 0; i < nb; i++) {
//         c = extract_head(l);
//         // printf("c: %s\n", (char*) c->content);
// 		printf("%s \n", (char*)  c-> content );
//     }
//     return 0;
// }