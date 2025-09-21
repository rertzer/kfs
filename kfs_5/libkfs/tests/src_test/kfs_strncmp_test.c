#include "kfs_strncmp.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

Test(kfs_strncmp, null_null_zero) {
	char*  s1 = NULL;
	char*  s2 = NULL;
	size_t n = 0;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_strncmp, null_null_ft, .signal = SIGSEGV) {
	char*  s1 = NULL;
	char*  s2 = NULL;
	size_t n = 42;
	kfs_strncmp(s1, s2, n);
}

Test(kfs_strncmp, identical__len_zero) {
	char*  s1 = "abcdef";
	char*  s2 = "abcdef";
	size_t n = 0;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_strncmp, identical_full_len) {
	char*  s1 = "abcdef";
	char*  s2 = "abcdef";
	size_t n = 6;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_strncmp, different_full_len) {
	char*  s1 = "abcDEF";
	char*  s2 = "abcdef";
	size_t n = 6;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_strncmp, accent_full_len) {
	char*  s1 = "abcdéf";
	char*  s2 = "abcdef";
	size_t n = 6;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_strncmp, identical_on_half_len) {
	char*  s1 = "abcDEF";
	char*  s2 = "abcdef";
	size_t n = 3;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_strncmp, identical_to_long) {
	char* s1 = malloc(7);
	char* s2 = "abcdef";
	strncpy(s1, s2, 7);
	size_t n = 1024;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
	free(s1);
}

Test(kfs_strncmp, to_short_string) {
	char* s1 = malloc(7);
	char* s2 = "abcdefghijklmnopqrstuvwxyz";
	strncpy(s1, s2, 7);
	size_t n = 24;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
	free(s1);
}

Test(kfs_strncmp, s2_to_short_string) {
	char* s2 = malloc(7);
	char* s1 = "abcdefghijklmnopqrstuvwxyz";
	strncpy(s2, s1, 7);
	size_t n = 24;
	int	   k = kfs_strncmp(s1, s2, n);
	int	   m = strncmp(s1, s2, n);
	cr_assert(k == m);
	free(s2);
}
