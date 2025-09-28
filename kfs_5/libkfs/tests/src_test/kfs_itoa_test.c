#include "kfs_itoa.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"
#include "stdio.h"

void eraseBuffer(char* buffer) {
	for (size_t i = 0; i < 64; ++i) {
		buffer[i] = '=';
	}
}

// Test(kfs_itoa, null_buffer, .signal = SIGSEGV) {
// 	kfs_itoa(0, NULL, 0);
// }

Test(kfs_itoa, zero_base10) {
	char  buffer[64];
	char* b = kfs_itoa(0, buffer, 10);
	cr_assert(b == buffer);
	cr_assert(0 == strcmp(buffer, "0"));
}

Test(kfs_itoa, onedigit_base10) {
	char  buffer[64];
	int	  values[] = {1, 2, 3, 5, 9};
	char* expected[] = {"1", "2", "3", "5", "9"};
	for (size_t i = 0; i < 5; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, 10);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "b: %s, e: %s\n", buffer, expected[i]);
	}
}

Test(kfs_itoa, manydigit_base10) {
	char  buffer[64];
	int	  values[] = {11, 22, 63, 580, 1290};
	char* expected[] = {"11", "22", "63", "580", "1290"};
	for (size_t i = 0; i < 5; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, 10);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "b: %s, e: %s\n", buffer, expected[i]);
	}
}
