#ifndef LIST_HEAD_H
#define LIST_HEAD_H

#include <stdbool.h>
#include <stddef.h>

#define LIST_HEAD_SIZE sizeof(list_head_t)

typedef struct {
	void* next;
	void* prev;
} list_head_t;

typedef struct {
	list_head_t lh;
	int			payload1;
	int			payload2;
} test_t;

void   list_head_init(list_head_t* lh);
void   list_add(void* new_list, void* prev_list);
void   list_add_tail(void* new_list, void* next_list);
void   list_del(void* list);
void   list_del_offset(void* list, size_t offset);
void*  list_extract(void* list);
void*  list_extract_offset(void* list, size_t offset);
bool   list_empty(list_head_t* lh);
size_t list_size(list_head_t* lh);
void   list_for_each(list_head_t* lh, void (*f)(void*), size_t offset);
void   list_head_test();
#endif
