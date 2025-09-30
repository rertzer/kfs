#include "kfs_int_math.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "criterion.h"

Test(int_math, kfs_min) {
	int low_values[] = {-22222, -42, 0, 42, 2100888666};
	int high_values[] = {-42, 0, 12, 422, 2100888667};
	for (size_t i = 0; i < 5; ++i) {
		cr_assert(kfs_min(low_values[i], high_values[i]) == low_values[i]);
		cr_assert(kfs_min(high_values[i], low_values[i]) == low_values[i]);
	}
}

Test(int_math, kfs_max) {
	int low_values[] = {-22222, -42, 0, 42, 2100888666};
	int high_values[] = {-42, 0, 12, 422, 2100888667};
	for (size_t i = 0; i < 5; ++i) {
		cr_assert(kfs_max(low_values[i], high_values[i]) == high_values[i]);
		cr_assert(kfs_max(high_values[i], low_values[i]) == high_values[i]);
	}
}

Test(int_math, kfs_abs) {
	int			 signed_values[] = {-22222, -42, 0, 42, 2100888666};
	unsigned int abs_values[] = {22222, 42, 0, 42, 2100888666};
	for (size_t i = 0; i < 5; ++i) {
		cr_assert(kfs_abs(signed_values[i]) == abs_values[i]);
	}
}
