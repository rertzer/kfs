#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"

#include "kfs_memset.h"

static void kfs_memset_assert(char* str, char* expected, char c, size_t len);

static void kfs_memset_assert(char* str, char* expected, char c, size_t len) {
	void* ret = kfs_memset(str, c, len);
	cr_assert(ret == str);
	int diff = strncmp(str, expected, strlen(str));
	cr_assert(diff == 0);
}

Test(kfs_memset, all_void) {
	void* k = kfs_memset(NULL, '\0', 0);
	cr_assert(k == NULL);
}

Test(kfs_memset, setNULL, .signal = SIGSEGV) {
	void* k = kfs_memset(NULL, '\0', 42);
	cr_assert(k == NULL);
}

Test(kfs_memset, size_zero) {
	char* str = "abcdef";
	char* expected = "abcdef";

	kfs_memset_assert(str, expected, '\0', 0);
}

Test(kfs_memset, replace_first) {
	char  str[] = "abcdef";
	char* expected = "Xbcdef";
	kfs_memset_assert(str, expected, 'X', 1);
}

Test(kfs_memset, replace_few) {
	char  str[] = "abcdefghijklmnopq";
	char* expected = "GGGGGGGGijklmnopq";
	kfs_memset_assert(str, expected, 'G', 8);
}
