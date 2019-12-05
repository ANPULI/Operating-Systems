#include <stack.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	char* mywords[4] = {"The Force", "is", "strong", "within this Jedi"};
	int i;
	int nb;
	
	struct list_type *l;
    struct cell_type *c;
    init_list(l);

    printf("starting\n");
	
	for(i = 0; i < 4; i++)
		insert_head(l, mywords[i]);
    // printf(l->head->content);

	nb = list_size(l);
    printf("size: %d\n", nb);

    printf("head: %s\n", (char *)l->head->content);

	for(i = 0; i < nb; i++) {
        c = extract_head(l);
        // printf("c: %s\n", (char*) c->content);
		printf("%s \n", (char*)  c-> content );
    }

	return EXIT_SUCCESS;
}