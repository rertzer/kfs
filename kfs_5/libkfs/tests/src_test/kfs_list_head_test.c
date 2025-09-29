#include "kfs_list_head.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

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

	free(nlh);
}
