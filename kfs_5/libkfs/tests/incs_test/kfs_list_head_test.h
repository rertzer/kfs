#ifndef KFS_LIST_HEAD_TEST_H
#define KFS_LIST_HEAD_TEST_H

#include "kfs_list_head.h"

typedef struct {
	list_head_t lh;
	int			payload1;
	int			payload2;
} test_t;

#endif
