#ifndef LIST_HEAD_H
#define LIST_HEAD_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	void* next;
	void* prev;
} list_head_t;

void   list_head_init(list_head_t* lh);
void   list_add(void* new_list, void* prev_list);
void   list_add_tail(void* new_list, void* next_list);
void   list_del(void* list);
void*  list_extract(void* list);
bool   list_empty(list_head_t* lh);
size_t list_size(list_head_t* lh);
void   list_for_each(void* lh, void (*f)(void*));
#endif
