#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

#include "kfs_ctype.h"
#include "kfs_ctype_test.h"

static void kfs_type_test(istype_t fun, int* input, size_t nb, bool output);
static void kfs_type_lst_test(istype_t fun, int* input, size_t nb, int* output);

static void kfs_type_test(istype_t fun, int* input, size_t nb, bool output) {
	for (size_t i = 0; i < nb; ++i) {
		int answer = fun(input[i]);
		cr_assert(answer == output);
	}
}
static void kfs_type_lst_test(istype_t fun, int* input, size_t nb, int* output) {
	for (size_t i = 0; i < nb; ++i) {
		int answer = fun(input[i]);
		cr_assert(answer == output[i]);
	}
}
Test(ctype, isspace_space) {
	int testchars[] = {' ', '\t', '\n', '\v', '\f', '\r'};
	kfs_type_test(kfs_isspace, testchars, 6, true);
}

Test(ctype, isspace_nospace) {
	int testchars[9] = {0, 2, 8, 14, 42, 120, 127, 212, 255};

	kfs_type_test(kfs_isspace, testchars, 9, false);
}

Test(ctype, isdigit_digits) {
	int testchars[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	kfs_type_test(kfs_isdigit, testchars, 10, true);
}

Test(ctype, isdigit_nodigits) {
	int testchars[10] = {'a', '-', 'B', 127, 220, 0, '%', '_', '*', '+'};
	kfs_type_test(kfs_isdigit, testchars, 10, false);
}

Test(ctype, isupper_upper) {
	int testchars[10] = {'A', 'B', 'E', 'G', 'I', 'L', 'N', 'O', 'W', 'Z'};
	kfs_type_test(kfs_isupper, testchars, 10, true);
}

Test(ctype, isupper_notupper) {
	int testchars[10] = {'a', '-', '9', 127, 220, 0, '%', '_', 'z', '!'};
	kfs_type_test(kfs_isupper, testchars, 10, false);
}

Test(ctype, islower_lower) {
	int testchars[10] = {'a', 'b', 'e', 'g', 'i', 'l', 'n', 'o', 'w', 'z'};
	kfs_type_test(kfs_islower, testchars, 10, true);
}

Test(ctype, islower_notlower) {
	int testchars[10] = {'A', '-', '9', 127, 220, 0, '%', '_', 'Z', '!'};
	kfs_type_test(kfs_islower, testchars, 10, false);
}

Test(ctype, isalnum_notnumb) {
	int testchars[10] = {')', '-', '>', 127, 220, 0, '%', '_', '{', '!'};
	kfs_type_test(kfs_isalnum, testchars, 10, false);
}

Test(ctype, isaplpha_notalpha) {
	int testchars[10] = {'0', '-', '9', 127, 220, 0, '%', '_', ',', '!'};
	kfs_type_test(kfs_isalpha, testchars, 10, false);
}

Test(ctype, isalnum_numb) {
	int testchars[10] = {'0', '1', '2', '9', 'a', 'A', 'z', 'Z', 'P', 'q'};
	kfs_type_test(kfs_isalnum, testchars, 10, true);
}

Test(ctype, isaplpha_alpha) {
	int testchars[10] = {'a', 'b', 'f', 'u', 'z', 'A', 'D', 'G', 'M', 'Z'};
	kfs_type_test(kfs_isalnum, testchars, 10, true);
}

Test(ctype, toupper) {
	int testchars[10] = {'a', 'b', '5', '_', ' ', 220, 0, 'G', 'M', 'Z'};
	int expected[10] = {'A', 'B', '5', '_', ' ', 220, 0, 'G', 'M', 'Z'};

	kfs_type_lst_test(kfs_toupper, testchars, 10, expected);
}

Test(ctype, toulower) {
	int testchars[10] = {'a', 'b', '5', '_', ' ', 220, 0, 'G', 'M', 'Z'};
	int expected[10] = {'a', 'b', '5', '_', ' ', 220, 0, 'g', 'm', 'z'};
	kfs_type_lst_test(kfs_tolower, testchars, 10, expected);
}
