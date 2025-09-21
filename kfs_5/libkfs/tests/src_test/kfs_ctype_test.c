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
