#include "kfs_priority_queue.h"
#include <stdio.h>
#include "kfs_memset.h"
#include "stdbool.h"

static void			 bubble_up(priority_queue_t* pq, size_t index);
static void			 bubble_down(priority_queue_t* pq, size_t index);
static inline size_t get_parent_index(size_t index);
static inline size_t get_left_child_index(size_t index);
static inline size_t get_right_child_index(size_t index);
static inline bool	 is_leaf(priority_queue_t* pq, size_t index);
static inline void	 pq_swap(priority_queue_t* pq, size_t i, size_t j);

void pq_init(priority_queue_t* pq) {
	kfs_memset(pq, 0, sizeof(priority_queue_t));
}

void pq_add(priority_queue_t* pq, priority_t* p) {
	if (pq->size == PQ_MAX_SIZE - 1) {
		return;
	}
	pq->size += 1;
	pq->queue[pq->size] = *p;
	bubble_up(pq, pq->size);
}

uint32_t pq_get_min(priority_queue_t* pq) {
	return (pq->queue[1].time);
}

priority_t pq_extract_min(priority_queue_t* pq) {
	priority_t p = pq->queue[1];

	if (pq->size == 0) {
		return pq->queue[0];
	}
	pq->queue[1] = pq->queue[pq->size];
	bubble_down(pq, 1);
	pq->size -= 1;

	return p;
}

uint32_t pq_get_size(priority_queue_t* pq) {
	return (pq->size);
}

static void bubble_up(priority_queue_t* pq, size_t index) {
	if (index < 2) {
		return;
	}
	size_t parent_index = get_parent_index(index);
	if (pq->queue[parent_index].time > pq->queue[index].time) {
		priority_t tmp = pq->queue[parent_index];
		pq->queue[parent_index] = pq->queue[index];
		pq->queue[index] = tmp;
		bubble_up(pq, parent_index);
	}
}

static void bubble_down(priority_queue_t* pq, size_t index) {
	if (!is_leaf(pq, index)) {
		size_t child_index = get_left_child_index(index);
		size_t right_index = get_right_child_index(index);
		if (right_index <= pq->size && pq->queue[right_index].time < pq->queue[child_index].time) {
			child_index = right_index;
		}
		if (pq->queue[child_index].time < pq->queue[index].time) {
			pq_swap(pq, index, child_index);
			bubble_down(pq, child_index);
		}
	}
}

static inline size_t get_parent_index(size_t index) {
	return (index >> 1);
}

static inline size_t get_left_child_index(size_t index) {
	return (index << 1);
}

static inline size_t get_right_child_index(size_t index) {
	return ((index << 1) + 1);
}

static inline bool is_leaf(priority_queue_t* pq, size_t index) {
	return (index > (pq->size >> 1));
}
static inline void pq_swap(priority_queue_t* pq, size_t i, size_t j) {
	priority_t tmp = pq->queue[i];
	pq->queue[i] = pq->queue[j];
	pq->queue[j] = tmp;
}
