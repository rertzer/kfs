#include "kfs_memcmp.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

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

	cr_assert(k == m);
}

Test(kfs_memcmp, identical_strings_n_fulllength) {
	char*  s1 = "abcdef";
	char*  s2 = "abcdef";
	size_t n = 7;

	int k = kfs_memcmp(s1, s2, n);
	int m = memcmp(s1, s2, n);

	cr_assert(k == m);
}

Test(kfs_memcmp, identical_strings_n_uintmax, .signal = SIGSEGV) {
	char*  s1 = "abcdef";
	char*  s2 = "abcdef";
	size_t n = UINT_MAX;

	kfs_memcmp(s1, s2, n);
}

Test(kfs_memcmp, different_strings_n) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef"};
	char*  s2[] = {"fedcba", "afghij", "abged", "abcdeg"};
	size_t n = 7;

	for (size_t i = 0; i < sizeof(s1) / sizeof(s1[0]); ++i) {
		int k = kfs_memcmp(s1[i], s2[i], n);
		int m = memcmp(s1[i], s2[i], n);

		cr_assert(k == m);
	}
}

Test(kfs_memcmp, different_strings_UINTMAX) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char*  s2[] = {"fedcba", "afghij", "abged", "abcdeg", "abcdefgh", "abcdef", "abcd"};
	size_t n = UINT_MAX;

	for (size_t i = 0; i < sizeof(s1) / sizeof(s1[0]); ++i) {
		int k = kfs_memcmp(s1[i], s2[i], n);
		int m = memcmp(s1[i], s2[i], n);

		cr_assert(k == m);
	}
}

Test(kfs_memcmp, string_empty_UINTMAX) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char*  s2 = "";
	size_t n = UINT_MAX;

	for (size_t i = 0; i < sizeof(s1) / sizeof(s1[0]); ++i) {
		int k = kfs_memcmp(s1[i], s2, n);
		int m = memcmp(s1[i], s2, n);

		cr_assert(k == m);
	}
}

Test(kfs_memcmp, different_strings_nbeforediff) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char*  s2[] = {"fedcba", "afghij", "abged", "abcdeg", "abcdefgh", "abcdef", "abcd"};
	size_t n[] = {0, 1, 2, 5, 6, 3};

	for (size_t i = 0; i < sizeof(s1) / sizeof(s1[0]); ++i) {
		int k = kfs_memcmp(s1[i], s2[i], n[i]);
		int m = memcmp(s1[i], s2[i], n[i]);

		cr_assert(k == m);
	}
}

Test(kfs_memcmp, different_strings_nafterediff) {
	char*  s1[] = {"abcdef", "abcdef", "abcdef", "abcdef", "abcdéfgh", "abcdefghij", "abc"};
	char*  s2[] = {"fedcba", "afghij", "abged", "abcdeg", "abcdefgh", "abcdef", "abcd"};
	size_t n[] = {2, 3, 4, 6, 7, 4};

	for (size_t i = 0; i < sizeof(s1) / sizeof(s1[0]); ++i) {
		int k = kfs_memcmp(s1[i], s2[i], n[i]);
		int m = memcmp(s1[i], s2[i], n[i]);

		cr_assert(k == m);
	}
}
