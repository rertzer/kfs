#include "kfs_memset.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"

Test(kfs_memset, all_void) {
	void* k = kfs_memset(NULL, '\0', 0);
	cr_assert(k == NULL);
}

Test(kfs_memset, size_zero) {
	char* str = "abcdef";
	char* expected_str = "abcdef";
	void* returned_pointer = kfs_memset(str, '\0', 0);
	cr_assert(returned_pointer == str);
	int diff = strncmp(str, expected_str, strlen(str));
	printf("memset sizezero diff %d\n", diff);
	cr_assert(diff == 0);
}

Test(kfs_memset, replace_first) {
	char* str = malloc(7);
	strcpy(str, "abcdef");
	char* expected_str = "Xbcdef";
	void* returned_pointer = kfs_memset(str, 'X', 1);
	cr_assert(returned_pointer == str);
	int diff = strncmp(str, expected_str, strlen(str));
	printf("memset sizezero diff %d %s\n", diff, str);
	cr_assert(diff == 0);
	free(str);
}
