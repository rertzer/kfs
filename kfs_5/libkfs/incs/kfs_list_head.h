#ifndef KFS_LIST_HEAD_H
#define KFS_LIST_HEAD_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	void* next;
	void* prev;
} list_head_t;

void   list_head_init(list_head_t* lh);
void   list_add(list_head_t* new_list, list_head_t* prev_list);
void   list_add_tail(list_head_t* new_list, list_head_t* next_list);
void   list_del(void* list);
void*  list_extract(list_head_t* lh, void* list);
bool   list_empty(list_head_t* lh);
size_t list_size(list_head_t* lh);
void   list_for_each(list_head_t* lh, void (*f)(void*));
#endif
