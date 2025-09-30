#include "kfs_itoa.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "criterion.h"

void eraseBuffer(char* buffer) {
	for (size_t i = 0; i < 64; ++i) {
		buffer[i] = '=';
	}
}

Test(kfs_itoa, null_buffer, .signal = SIGSEGV) {
	kfs_itoa(42, NULL, 10);
}

Test(kfs_itoa, zero_base10) {
	char  buffer[64];
	char* b = kfs_itoa(0, buffer, 10);
	cr_assert(b == buffer);
	cr_assert(0 == strcmp(buffer, "0"));
}

Test(kfs_itos, wrongbase) {
	char  buffer[64];
	char* b = kfs_itoa(42, buffer, 42);
	cr_assert(b == NULL);
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
	int	  values[] = {11, 22, 63, 580, 1290, 2147483647};
	char* expected[] = {"11", "22", "63", "580", "1290", "2147483647"};
	for (size_t i = 0; i < 6; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, 10);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "b: %s, e: %s\n", buffer, expected[i]);
	}
}

Test(kfs_itoa, negativedigit_base10) {
	char  buffer[64];
	int	  values[] = {-42, -22, -63, -580, -1290, -2147483648};
	char* expected[] = {"-42", "-22", "-63", "-580", "-1290", "-2147483648"};
	for (size_t i = 0; i < 6; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, 10);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "b: %s, e: %s\n", buffer, expected[i]);
	}
}

Test(kfs_itoa, base8) {
	char		 buffer[64];
	unsigned int values[] = {42, 2222222222, 63, 1000000580, 2147483647, 2147483648};
	char*		 expected[] = {"52", "20435065616", "77", "7346546104", "17777777777", "20000000000"};
	for (size_t i = 0; i < 6; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, 8);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "b: %s, e: %s\n", buffer, expected[i]);
	}
}

Test(kfs_itoa, base16) {
	char		 buffer[64];
	unsigned int values[] = {42, 2222222222, 63, 1000000580, 2147483647, 2147483648, 11259375};
	char*		 expected[] = {"2a", "84746b8e", "3f", "3b9acc44", "7fffffff", "80000000", "abcdef"};
	for (size_t i = 0; i < 7; ++i) {
		eraseBuffer(buffer);
		char* b = kfs_itoa(values[i], buffer, 16);

		cr_assert(b == buffer);
		cr_assert(0 == strcmp(buffer, expected[i]), "b: %s, e: %s\n", buffer, expected[i]);
	}
}
