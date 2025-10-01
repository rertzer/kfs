#ifndef KFS_LIST_HEAD_TEST_H
#define KFS_LIST_HEAD_TEST_H

#include "kfs_list_head.h"

#define TO_OFFSET (2 * sizeof(int))

typedef struct {
	list_head_t lh;
	int			payload1;
	int			payload2;
} test_t;

typedef struct {
	int			payload1;
	int			payload2;
	list_head_t lh;
} test_offset_t;
#endif
