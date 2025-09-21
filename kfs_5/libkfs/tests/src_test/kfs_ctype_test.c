#include "kfs_ctype.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

Test(ctype, isspace_space) {
	int testchars[6] = {' ', '\t', '\n', '\v', '\f', '\r'};
	for (int i = 0; i < 6; ++i) {
		int answer = kfs_isspace(testchars[i]);
		cr_assert(answer == true);
	}
}

Test(ctype, isspace_nospace) {
	int testchars[9] = {0, 2, 8, 14, 42, 120, 127, 212, 255};
	for (int i = 0; i < 9; ++i) {
		int answer = kfs_isspace(testchars[i]);
		cr_assert(answer == false);
	}
}

Test(ctype, isdigit_digits) {
	int testchars[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isdigit(testchars[i]);
		cr_assert(answer == true);
	}
}

Test(ctype, isdigit_nodigits) {
	int testchars[10] = {'a', '-', 'B', 127, 220, 0, '%', '_', '*', '+'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isdigit(testchars[i]);
		cr_assert(answer == false);
	}
}
