#include "kfs_list_head.h"
#include <stdlib.h>

void list_head_init(list_head_t* lh) {
	lh->next = lh;
	lh->prev = lh;
}

void list_add(void* v_new_list, void* v_prev_list) {
	list_head_t* new_list = v_new_list;
	list_head_t* prev_list = v_prev_list;

	new_list->prev = prev_list;
	new_list->next = prev_list->next;
	((list_head_t*)((list_head_t*)prev_list)->next)->prev = new_list;
	prev_list->next = new_list;
}

void list_add_tail(void* v_new_list, void* v_next_list) {
	list_head_t* new_list = v_new_list;
	list_head_t* next_list = v_next_list;

	((list_head_t*)((list_head_t*)next_list)->prev)->next = new_list;
	new_list->prev = next_list->prev;
	new_list->next = next_list;
	next_list->prev = new_list;
}

void list_del(void* v_list) {
	list_head_t* list = v_list;
	((list_head_t*)((list_head_t*)list)->prev)->next = list->next;
	((list_head_t*)((list_head_t*)list)->next)->prev = list->prev;
	free(list);
}

void* list_extract(void* v_list) {
	list_head_t* list = v_list;
	((list_head_t*)((list_head_t*)list)->prev)->next = list->next;
	((list_head_t*)((list_head_t*)list)->next)->prev = list->prev;
	list_head_init(list);
	return (list);
}

bool list_empty(list_head_t* lh) {
	if (lh->next == lh) {
		return (true);
	}
	return (false);
}

size_t list_size(list_head_t* lh) {
	size_t		 counter = 0;
	list_head_t* current = lh;

	while (current->next != lh) {
		++counter;
		current = current->next;
	}

	return (counter);
}
