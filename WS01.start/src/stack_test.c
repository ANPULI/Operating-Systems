#include <stack.h>
#include <list.h>
#include <stdio.h>
#include <stdlib.h>

// struct list_type* l;

int main(int argc, char** argv) {

	char* mywords[4] = {"The Force", "is", "strong", "within this Jedi"};
	int i;
	int nb;

	// l = malloc(sizeof(struct list_type*));

	init_stack();
	for(i = 0; i < 4; i++)
		push(mywords[i]);
	
	nb = size();
	
	// DISPLAY MAX STACK SIZE
	printf("MAX STACK SIZE: %d\n", STACK_SIZE);

	printf("The size of the stack is %d\n", nb);
	
	for(i = 0; i < nb; i++)
		printf("%s \n", (char*)pop());

	return EXIT_SUCCESS;
}
