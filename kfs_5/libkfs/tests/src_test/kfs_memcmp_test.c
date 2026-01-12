#include "kfs_memcmp.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

static bool sameSign(int k, int m);
static void kfs_memcmp_assert(char** s1, char** s2, size_t len, size_t nb);
static void kfs_memcmp_lst_assert(char** s1, char** s2, size_t* len, size_t nb);

static bool sameSign(int k, int m) {
	return ((k == 0 && m == 0) || (k < 0 && m < 0) || (k > 0 && m > 0));
}

static void kfs_memcmp_assert(char** s1, char** s2, size_t len, size_t nb) {
	for (size_t i = 0; i < nb; ++i) {
		int k = kfs_memcmp(s1[i], s2[i], len);
		int m = memcmp(s1[i], s2[i], len);

		cr_assert(sameSign(k, m));
	}
}
static void kfs_memcmp_lst_assert(char** s1, char** s2, size_t* len, size_t nb) {
	for (size_t i = 0; i < nb; ++i) {
		int k = kfs_memcmp(s1[i], s2[i], len[i]);
		int m = memcmp(s1[i], s2[i], len[i]);

		cr_assert(sameSign(k, m));
	}
}

Test(kfs_memcmp, null_null_zero) {
	char*  s1 = NULL;
	char*  s2 = NULL;
	size_t n = 0;
	int	   k = kfs_memcmp(s1, s2, n);
	int	   m = memcmp(s1, s2, n);
	cr_assert(k == m);
}

Test(kfs_memcmp, null_null_42, .signal = SIGSEGV) {
	char*  s1 = NULL;
	char*  s2 = NULL;
	size_t n = 42;
	kfs_memcmp(s1, s2, n);
}

Test(kfs_memcmp, null_empty_42, .signal = SIGSEGV) {
	char*  s1 = NULL;
	char*  s2 = "";
	size_t n = 42;
	kfs_memcmp(s1, s2, n);
}

Test(kfs_memcmp, empty_null_42, .signal = SIGSEGV) {
	char*  s1 = "";
	char*  s2 = NULL;
	size_t n = 42;
	kfs_memcmp(s1, s2, n);
}

Test(kfs_memcmp, empty_empty_0) {
	char*  s1 = "";
	char*  s2 = "";
	size_t n = 0;

	int k = kfs_memcmp(s1, s2, n);
	int m = memcmp(s1, s2, n);

	cr_assert(k == m);
}

Test(kfs_memcmp, empty_empty_42) {
	char*  s1 = "";
	char*  s2 = "";
	size_t n = 42;

	int k = kfs_memcmp(s1, s2, n);
	int m = memcmp(s1, s2, n);

	cr_assert(sameSign(k, m));
}

Test(kfs_memcmp, identical_strings_n_fulllength) {
	char*  s1 = "abcdef";
	char*  s2 = "abcdef";
	size_t n = 7;

	int k = kfs_memcmp(s1, s2, n);
	int m = memcmp(s1, s2, n);

	cr_assert(sameSign(k, m));
}

Test(kfs_memcmp, identical_strings_n_uintmax, .signal = SIGSEGV) {
	char*  s1 = "abcdef";
	char*  s2 = "abcdef";
	size_t n = UINT_MAX;

	kfs_memcmp(s1, s2, n);
}

Test(kfs_memcmp, different_strings_n) {
	char* s1[] = {"abcdef", "abcdef", "abcdef", "abcdef"};
	char* s2[] = {"fedcba", "afghij", "abged", "abcdeg"};
	kfs_memcmp_assert(s1, s2, 7, 4);
}

Test(kfs_memcmp, different_strings_UINTMAX) {
	char* s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char* s2[] = {"fedcba", "afghij", "abged", "abcdeg", "abcdefgh", "abcdef", "abcd"};

	kfs_memcmp_assert(s1, s2, UINT_MAX, 7);
}

Test(kfs_memcmp, string_empty_UINTMAX) {
	char* s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char* s2[] = {"", "", "", "", "", "", ""};

	kfs_memcmp_assert(s1, s2, UINT_MAX, 7);
}

Test(kfs_memcmp, different_strings_beforediff) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char*  s2[] = {"fedcba", "afghij", "abged", "abcdeg", "abcdefgh", "abcdef", "abcd"};
	size_t n[] = {0, 1, 2, 5, 4, 6, 3};

	kfs_memcmp_lst_assert(s1, s2, n, 7);
}

Test(kfs_memcmp, different_strings_afterediff) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char*  s2[] = {"fedcba", "afghij", "abged", "abcdeg", "abcdefgh", "abcdef", "abcd"};
	size_t n[] = {2, 3, 4, 6, 4, 7, 4};

	kfs_memcmp_lst_assert(s1, s2, n, 7);
}
