#include "kfs_memcpy.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

static void kfs_memcpy_assert(char* dest, char* expected, char* src, size_t len);

static void kfs_memcpy_assert(char* dest, char* expected, char* src, size_t len) {
	void* ptr = kfs_memcpy(dest, src, len);

	cr_assert(ptr == dest);
	int diff = strncmp(dest, expected, strlen(expected));
	cr_assert(diff == 0);
}

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
	char  dest[] = "............";
	char* expected_dest = "abcd........";
	char* src = "abcdefghijklmnopqrstuvwxyz";
	kfs_memcpy_assert(dest, expected_dest, src, 4);
}

Test(kfs_memcpy, full_copy) {
	char  dest[] = "............";
	char* expected_dest = "abcdefghijkl";
	char* src = "abcdefghijklmnopqrstuvwxyz";
	kfs_memcpy_assert(dest, expected_dest, src, 12);
}
