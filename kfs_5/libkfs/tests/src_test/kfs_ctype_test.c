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

Test(ctype, isupper_upper) {
	int testchars[10] = {'A', 'B', 'E', 'G', 'I', 'L', 'N', 'O', 'W', 'Z'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isupper(testchars[i]);
		cr_assert(answer == true);
	}
}

Test(ctype, isupper_notupper) {
	int testchars[10] = {'a', '-', '9', 127, 220, 0, '%', '_', 'z', '!'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isupper(testchars[i]);
		cr_assert(answer == false);
	}
}

Test(ctype, islower_lower) {
	int testchars[10] = {'a', 'b', 'e', 'g', 'i', 'l', 'n', 'o', 'w', 'z'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_islower(testchars[i]);
		cr_assert(answer == true);
	}
}

Test(ctype, islower_notlower) {
	int testchars[10] = {'A', '-', '9', 127, 220, 0, '%', '_', 'Z', '!'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_islower(testchars[i]);
		cr_assert(answer == false);
	}
}

Test(ctype, isalnum_notnumb) {
	int testchars[10] = {')', '-', '>', 127, 220, 0, '%', '_', '{', '!'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isalnum(testchars[i]);
		cr_assert(answer == false);
	}
}

Test(ctype, isaplpha_notalpha) {
	int testchars[10] = {'0', '-', '9', 127, 220, 0, '%', '_', ',', '!'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isalpha(testchars[i]);
		cr_assert(answer == false);
	}
}

Test(ctype, isalnum_numb) {
	int testchars[10] = {'0', '1', '2', '9', 'a', 'A', 'z', 'Z', 'P', 'q'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isalnum(testchars[i]);
		cr_assert(answer == true);
	}
}

Test(ctype, isaplpha_alpha) {
	int testchars[10] = {'a', 'b', 'f', 'u', 'z', 'A', 'D', 'G', 'M', 'Z'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_isalpha(testchars[i]);
		cr_assert(answer == true);
	}
}

Test(ctype, toupper) {
	int testchars[10] = {'a', 'b', '5', '_', ' ', 220, 0, 'G', 'M', 'Z'};
	int expected[10] = {'A', 'B', '5', '_', ' ', 220, 0, 'G', 'M', 'Z'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_toupper(testchars[i]);
		cr_assert(answer == expected[i]);
	}
}

Test(ctype, toulower) {
	int testchars[10] = {'a', 'b', '5', '_', ' ', 220, 0, 'G', 'M', 'Z'};
	int expected[10] = {'a', 'b', '5', '_', ' ', 220, 0, 'g', 'm', 'z'};
	for (int i = 0; i < 10; ++i) {
		int answer = kfs_tolower(testchars[i]);
		cr_assert(answer == expected[i]);
	}
}
