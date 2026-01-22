#ifndef KFS_PRIORITY_QUEUE
#define KFS_PRIORITY_QUEUE

#include "stdint.h"

// #ifndef JROS
// #define proc_t uint32_t*
// #else
typedef struct proc_s proc_t;
// #endif

#define PQ_MAX_SIZE 1024

typedef struct {
	uint32_t time;
	proc_t*	 proc;
} priority_t;

typedef struct {
	priority_t queue[PQ_MAX_SIZE];
	uint32_t   size;
} priority_queue_t;

void	   pq_init(priority_queue_t* pq);
void	   pq_add(priority_queue_t* pq, priority_t* p);
uint32_t   pq_get_size(priority_queue_t* pq);
uint32_t   pq_get_min(priority_queue_t* pq);
priority_t pq_extract_min(priority_queue_t* pq);

#endif
