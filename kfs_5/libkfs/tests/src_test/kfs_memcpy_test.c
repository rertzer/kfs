#include "kfs_memcpy.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

Test(kfs_memcpy, null_null_zero) {
	char*  dest = NULL;
	char*  src = NULL;
	size_t n = 0;
	void*  ptr = kfs_memcpy(dest, src, n);

	cr_assert(ptr == NULL);
}

Test(kfs_memcpy, null_null_42, .signal = SIGSEGV) {
	char*  dest = NULL;
	char*  src = NULL;
	size_t n = 42;
	void*  ptr = kfs_memcpy(dest, src, n);
	cr_assert(ptr == dest);
}

Test(kfs_memcpy, partial_copy) {
	char* dest = malloc(13);
	strncpy(dest, "............", 12);
	char*  expected_dest = "abcd........";
	char*  src = "abcdefghijklmnopqrstuvwxyz";
	size_t n = 4;
	void*  ptr = kfs_memcpy(dest, src, n);

	cr_assert(ptr == dest);
	int diff = strncmp(dest, expected_dest, strlen(expected_dest));
	cr_assert(diff == 0);
	free(dest);
}

Test(kfs_memcpy, full_copy) {
	char* dest = malloc(13);
	strncpy(dest, "............", 12);
	char*  expected_dest = "abcdefghijkl";
	char*  src = "abcdefghijklmnopqrstuvwxyz";
	size_t n = 12;
	void*  ptr = kfs_memcpy(dest, src, n);

	cr_assert(ptr == dest);
	int diff = strncmp(dest, expected_dest, strlen(expected_dest));
	cr_assert(diff == 0);
}
