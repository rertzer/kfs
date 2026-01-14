#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

#include "kfs_strncmp.h"

static void kfs_strncmp_assert(char* s1, char* s2, size_t len);

static void kfs_strncmp_assert(char* s1, char* s2, size_t len) {
	int k = kfs_strncmp(s1, s2, len);
	int m = strncmp(s1, s2, len);
	cr_assert(k == m);
}

Test(kfs_strncmp, null_null_zero) {
	char* s1 = NULL;
	char* s2 = NULL;
	kfs_strncmp(s1, s2, 0);
}

Test(kfs_strncmp, null_null_ft, .signal = SIGSEGV) {
	char*  s1 = NULL;
	char*  s2 = NULL;
	size_t n = 42;
	kfs_strncmp(s1, s2, n);
}

Test(kfs_strncmp, identical__len_zero) {
	char* s1 = "abcdef";
	char* s2 = "abcdef";

	kfs_strncmp_assert(s1, s2, 0);
}

Test(kfs_strncmp, identical_full_len) {
	char* s1 = "abcdef";
	char* s2 = "abcdef";
	kfs_strncmp_assert(s1, s2, 6);
}

Test(kfs_strncmp, different_full_len) {
	char* s1 = "abcDEF";
	char* s2 = "abcdef";
	kfs_strncmp_assert(s1, s2, 6);
}

Test(kfs_strncmp, accent_full_len) {
	char* s1 = "abcdéf";
	char* s2 = "abcdef";
	kfs_strncmp_assert(s1, s2, 6);
}

Test(kfs_strncmp, identical_on_half_len) {
	char* s1 = "abcDEF";
	char* s2 = "abcdef";
	kfs_strncmp_assert(s1, s2, 3);
}

Test(kfs_strncmp, identical_to_long) {
	char* s1 = "abcdef";
	kfs_strncmp_assert(s1, s1, 1024);
}

Test(kfs_strncmp, to_short_string) {
	char* s1 = "abcdefghijklmnopqrstuvwxyz";
	kfs_strncmp_assert(s1, s1, 24);
}

Test(kfs_strncmp, s2_to_short_string) {
	char* s1 = "abcdefghijklmnopqrstuvwxyz";
	kfs_strncmp_assert(s1, s1, 24);
}
