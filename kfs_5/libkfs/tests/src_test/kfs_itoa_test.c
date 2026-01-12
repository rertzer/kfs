#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "criterion.h"

#include "kfs_itoa.h"

static void kfs_itos_assert(int* values, char** expected, int base, size_t nb);
static void eraseBuffer(char* buffer);

static void kfs_itos_assert(int* values, char** expected, int base, size_t nb) {
	char buffer[64];

	for (size_t i = 0; i < nb; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, base);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "buffer: %s, expected: %s\n", buffer, expected[i]);
	}
}

void eraseBuffer(char* buffer) {
	for (size_t i = 0; i < 64; ++i) {
		buffer[i] = '=';
	}
}

Test(kfs_itoa, null_buffer_must_segfault, .signal = SIGSEGV) {
	kfs_itoa(42, NULL, 10);
}

Test(kfs_itoa, zero_base10) {
	char  buffer[64];
	char* b = kfs_itoa(0, buffer, 10);
	cr_assert(b == buffer);
	cr_assert(0 == strcmp(buffer, "0"));
}

Test(kfs_itos, wrongbase_returns_NULL) {
	char  buffer[64];
	char* b = kfs_itoa(42, buffer, 42);
	cr_assert(b == NULL);
}

Test(kfs_itoa, onedigit_base10) {
	int	  values[] = {1, 2, 3, 5, 9};
	char* expected[] = {"1", "2", "3", "5", "9"};
	kfs_itos_assert(values, expected, 10, 5);
}

Test(kfs_itoa, manydigit_base10) {
	int	  values[] = {11, 22, 63, 580, 1290, 2147483647};
	char* expected[] = {"11", "22", "63", "580", "1290", "2147483647"};

	kfs_itos_assert(values, expected, 10, 6);
}

Test(kfs_itoa, negativedigit_base10) {
	int	  values[] = {-42, -22, -63, -580, -1290, -2147483648};
	char* expected[] = {"-42", "-22", "-63", "-580", "-1290", "-2147483648"};
	kfs_itos_assert(values, expected, 10, 5);
}

Test(kfs_itoa, base8) {
	unsigned int values[] = {42, 2222222222, 63, 1000000580, 2147483647, 2147483648};
	char*		 expected[] = {"52", "20435065616", "77", "7346546104", "17777777777", "20000000000"};
	kfs_itos_assert((int*)values, expected, 8, 6);
}

Test(kfs_itoa, base16) {
	unsigned int values[] = {42, 2222222222, 63, 1000000580, 2147483647, 2147483648, 11259375};
	char*		 expected[] = {"2a", "84746b8e", "3f", "3b9acc44", "7fffffff", "80000000", "abcdef"};
	kfs_itos_assert((int*)values, expected, 16, 7);
}
