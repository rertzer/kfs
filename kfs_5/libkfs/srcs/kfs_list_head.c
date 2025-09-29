#include "kfs_list_head.h"

void list_head_init(list_head_t* lh) {
	lh->next = lh;
	lh->prev = lh;
}

void list_add(list_head_t* new_list, list_head_t* prev_list) {}
