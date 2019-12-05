#include <list.h>
#include <stdlib.h>
#include <stdio.h>


void init_list(struct list_type *l) {
	l->head = NULL;
	l->tail = NULL;
}
	
	
void insert_head(struct list_type *l, void* element) {
	cell* new_cell = malloc(sizeof(cell));
	new_cell->content = element;
	new_cell->previous = NULL;
	new_cell->next = l->head;
	if (l->head != NULL)
		l->head->previous = new_cell;
	l->head = new_cell;
	if (l->tail == NULL)
		l->tail = l->head;
}


void* extract_head(struct list_type *l) {
	/* TODO */
	if (l->head == NULL)
		return NULL;
	cell* h = l->head;
	cell* new_head = l->head->next;
	if (new_head == NULL)
		l->tail = NULL;
	else
		new_head->previous = NULL;
	free(l->head);
	l->head = new_head;
	return h;
}


void* extract_tail(struct list_type *l) {
	/* TODO */
	if (l->tail == NULL)
		return NULL;
	cell* t = l->tail;
	cell* new_tail = l->tail->previous;
	if (new_tail == NULL)
		l->head = NULL;
	else
		new_tail->next = NULL;
	free(l->tail);
	l->tail = new_tail;
	return t;
}


int list_size(struct list_type *l) {
	/* TODO */
	int count = 0;
	cell* ic = l->head;  //stands for iterator_cell
	while (ic != NULL) {
		count++;
		ic = ic->next;
	}
	return count;
}
	