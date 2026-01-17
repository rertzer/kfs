#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"

#include "kfs_priority_queue.h"

static void pq_test_feed(priority_queue_t* pq) {
	pq_init(pq);
	priority_t p;

	for (size_t i = 1; i < PQ_MAX_SIZE; ++i) {
		uint32_t value = PQ_MAX_SIZE - i;
		p.time = value;
		p.proc = NULL;
		pq_add(pq, &p);
	}
}

Test(priority_queue, pq_init) {
	priority_queue_t pq;
	pq_init(&pq);

	cr_assert(pq.size == 0);
	for (size_t i = 0; i < PQ_MAX_SIZE; ++i) {
		cr_assert(pq.queue[i].time == 0);
		cr_assert(pq.queue[i].proc == 0);
	}
}

Test(priority_queue, pq_add_and_get_min) {
	priority_queue_t pq;
	pq_init(&pq);
	priority_t p;

	for (size_t i = 1; i < PQ_MAX_SIZE; ++i) {
		uint32_t value = PQ_MAX_SIZE - i;
		p.time = value;
		p.proc = NULL;
		pq_add(&pq, &p);
		cr_assert(pq.size == i);
		size_t min = pq_get_min(&pq);
		cr_assert(min == value, "turn %lu, value: %lu, expected: %u", i, min, value);
	}
}

Test(priority_queue, overload_the_queue) {
	priority_queue_t pq;
	priority_t		 p;

	pq_test_feed(&pq);
	cr_assert(pq.size = PQ_MAX_SIZE - 1);
	pq_add(&pq, &p);

	cr_assert(pq.size = PQ_MAX_SIZE - 1);
}

Test(priority_queue, pq_extract_min) {
	priority_queue_t pq;
	priority_t		 p;

	pq_test_feed(&pq);
	cr_assert(pq.size = PQ_MAX_SIZE - 1);
	for (size_t i = 1; i < PQ_MAX_SIZE; ++i) {
		p = pq_extract_min(&pq);
		uint32_t expected_size = PQ_MAX_SIZE - i - 1;
		cr_assert(pq.size == expected_size, "round %lu, size is %u instead of %u", i, pq.size, expected_size);
		cr_assert(p.time == i);
	}
}

Test(priority_queue, get_size) {
	priority_queue_t pq;
	priority_t		 p;

	pq_init(&pq);
	cr_assert(pq_get_size(&pq) == 0);
	for (size_t i = 1; i < PQ_MAX_SIZE; ++i) {
		uint32_t value = PQ_MAX_SIZE - i;
		p.time = value;
		p.proc = NULL;
		pq_add(&pq, &p);
		cr_assert(pq_get_size(&pq) == i);
	}
}
