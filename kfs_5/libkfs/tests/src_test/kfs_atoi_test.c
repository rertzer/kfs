#include "kfs_atoi.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

Test(kfs_atoi, null_string, .signal = SIGSEGV) {
	kfs_atoi(NULL);
}

Test(kfs_atoi, empty_string) {
	int nb = kfs_atoi("");

	cr_assert(nb == 0);
}

Test(kfs_atoi, zero_value) {
	int nb = kfs_atoi("0");

	cr_assert(nb == 0);
}

Test(kfs_atoi, nan) {
	char* str_values[] = {"a", "abc1234", ")(42)", "trois"};

	for (size_t i = 0; i < 4; ++i) {
		int nb = kfs_atoi(str_values[i]);
		cr_assert(nb == 0);
	}
}
Test(kfs_atoi, positive_values) {
	char* str_values[] = {"1", "+2", "+10", "42", "2147483647", "2147483648"};
	int	  num_values[] = {1, 2, 10, 42, 2147483647, -2147483648};

	for (size_t i = 0; i < 6; ++i) {
		int nb = kfs_atoi(str_values[i]);
		cr_assert(nb == num_values[i]);
	}
}

Test(kfs_atoi, negative_values) {
	char* str_values[] = {"-0", "-1", "-2", "-10", "-42", "-2147483648", "-2147483649"};
	int	  num_values[] = {0, -1, -2, -10, -42, -2147483648, 2147483647};

	for (size_t i = 0; i < 7; ++i) {
		int nb = kfs_atoi(str_values[i]);
		cr_assert(nb == num_values[i]);
	}
}

Test(kfs_atoi, nb_and_crap) {
	char* str_values[] = {"-0++", "-1?", "2\\4", "-10,craps12", "42.42", "21+21", "-2147483648{0}", "2147483647,iopp"};
	int	  num_values[] = {0, -1, 2, -10, 42, 21, -2147483648, 2147483647};

	for (size_t i = 0; i < 8; ++i) {
		int nb = kfs_atoi(str_values[i]);
		cr_assert(nb == num_values[i]);
	}
}
Test(kfs_atoi, mixed_signs_and_spaces) {
	char* str_values[] = {"--1", " +2", "    -10", "    ---42++", "+ 2147483647", "-+-+2147483647"};
	int	  num_values[] = {0, 2, -10, 0, 0, 0, 0};

	for (size_t i = 0; i < 6; ++i) {
		int nb = kfs_atoi(str_values[i]);
		cr_assert(nb == num_values[i]);
	}
}
