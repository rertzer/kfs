#include "kfs_list_head.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"
#include "kfs_list_head_test.h"

Test(list_head, init_head) {
	list_head_t lh;

	list_head_init(&lh);
	cr_expect(lh.prev == &lh);
	cr_expect(lh.next == &lh);
}

Test(list_head, add) {
	list_head_t lh;
	list_head_init(&lh);
	list_head_t* nlh = malloc(sizeof(list_head_t));

	list_add(nlh, &lh);

	cr_expect(lh.prev == nlh);
	cr_expect(lh.next == nlh);
	cr_expect(nlh->prev == &lh);
	cr_expect(nlh->next == &lh);

	list_head_t* nlh2 = malloc(sizeof(list_head_t));
	list_add(nlh2, nlh);

	cr_expect(lh.prev == nlh2);
	cr_expect(lh.next == nlh);
	cr_expect(nlh->prev == &lh);
	cr_expect(nlh->next == nlh2);
	cr_expect(nlh2->prev == nlh);
	cr_expect(nlh2->next == &lh);

	free(nlh);
	free(nlh2);
}

Test(list_head, add_tail) {
	list_head_t lh;
	list_head_init(&lh);
	list_head_t* nlh = malloc(sizeof(list_head_t));

	list_add_tail(nlh, &lh);

	cr_expect(lh.prev == nlh);
	cr_expect(lh.next == nlh);
	cr_expect(nlh->prev == &lh);
	cr_expect(nlh->next == &lh);

	list_head_t* nlh2 = malloc(sizeof(list_head_t));
	list_add_tail(nlh2, nlh);

	cr_expect(lh.prev == nlh);
	cr_expect(lh.next == nlh2);
	cr_expect(nlh->prev == nlh2);
	cr_expect(nlh->next == &lh);
	cr_expect(nlh2->prev == &lh);
	cr_expect(nlh2->next == nlh);

	list_head_t* nlh3 = malloc(sizeof(list_head_t));
	list_add_tail(nlh3, &lh);

	cr_expect(lh.prev == nlh3);
	cr_expect(lh.next == nlh2);
	cr_expect(nlh->prev == nlh2);
	cr_expect(nlh->next == nlh3);
	cr_expect(nlh2->prev == &lh);
	cr_expect(nlh2->next == nlh);
	cr_expect(nlh3->prev == nlh);
	cr_expect(nlh3->next == &lh);

	free(nlh);
	free(nlh2);
	free(nlh3);
}

Test(list_head, list_del_free, .signal = SIGABRT) {
	list_head_t* nlh = malloc(sizeof(list_head_t));
	list_head_init(nlh);
	list_del(nlh);

	free(nlh);
}

Test(list_head, list_del_ptr) {
	list_head_t lh;
	list_head_init(&lh);
	list_head_t* nlh = malloc(sizeof(list_head_t));
	list_add(nlh, &lh);

	list_del(nlh);

	cr_expect(lh.prev == &lh);
	cr_expect(lh.next == &lh);
}

Test(list_head, small_list_extract) {
	list_head_t lh;
	list_head_init(&lh);

	list_head_t* nlh;
	nlh = malloc(sizeof(list_head_t));
	list_add_tail(nlh, &lh);

	list_head_t* extract = list_extract(nlh);

	cr_expect(extract == nlh);
	cr_expect(extract->prev == extract);
	cr_expect(extract->next == extract);
	cr_expect(lh.next == &lh);
	cr_expect(lh.prev == &lh);

	free(nlh);
}

Test(list_head, list_extract) {
	list_head_t lh;
	list_head_init(&lh);

	list_head_t* nlh[5];
	for (size_t i = 0; i < 5; ++i) {
		nlh[i] = malloc(sizeof(list_head_t));
		list_add_tail(nlh[i], &lh);
	}

	list_head_t* extract = list_extract(nlh[3]);

	cr_expect(extract == nlh[3]);
	cr_expect(extract->prev == extract);
	cr_expect(extract->next == extract);
	cr_expect(nlh[2]->next == nlh[4]);
	cr_expect(nlh[4]->prev == nlh[2]);

	for (size_t i = 0; i < 5; ++i) {
		free(nlh[i]);
	}
}

Test(list_head, is_empty) {
	list_head_t lh;
	list_head_init(&lh);
	cr_expect(list_empty(&lh) == true);
}

Test(list_head, is_not_empty) {
	list_head_t lh;
	list_head_init(&lh);

	list_head_t* nlh = malloc(sizeof(list_head_t));
	list_add(nlh, &lh);
	cr_expect(list_empty(&lh) == false);

	free(nlh);
}

Test(list_head, null_size) {
	list_head_t lh;
	list_head_init(&lh);
	cr_expect(list_size(&lh) == 0);
}

Test(list_head, non_null_size) {
	list_head_t lh;
	list_head_init(&lh);

	list_head_t* nlh[42];
	for (size_t i = 0; i < 42; ++i) {
		nlh[i] = malloc(sizeof(list_head_t));
		list_add_tail(nlh[i], &lh);
	}

	cr_expect(list_size(&lh) == 42);

	for (size_t i = 0; i < 42; ++i) {
		free(nlh[i]);
	}
}

Test(list_head, run_on_struct) {
	list_head_t lh;
	list_head_init(&lh);

	test_t* tt[42];
	for (size_t i = 0; i < 42; ++i) {
		tt[i] = malloc(sizeof(test_t));
		list_add_tail(tt[i], &lh);
	}
	test_t* current = lh.next;
	while (current != (test_t*)&lh) {
		current->payload2 = 42;
		current = current->lh.next;
	}

	current = lh.next;
	cr_expect(list_size(&lh) == 42);
	for (size_t i = 0; i < 42; ++i) {
		cr_assert(current->payload2 == 42, "index %zu\n", i);
	}
	for (size_t i = 0; i < 42; ++i) {
		free(tt[i]);
	}
}
