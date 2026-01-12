#include "kfs_atoi.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include "criterion.h"

static void kfs_atoi_assert_many(char** input, int* output, size_t nb);

static void kfs_atoi_assert_many(char** input, int* output, size_t nb) {
	for (size_t i = 0; i < nb; ++i) {
		cr_assert(kfs_atoi(input[i]) == output[i]);
	}
}

Test(kfs_atoi, null_string_must_segfault, .signal = SIGSEGV) {
	kfs_atoi(NULL);
}

Test(kfs_atoi, empty_string_returns_zero) {
	int nb = kfs_atoi("");

	cr_assert(nb == 0);
}

Test(kfs_atoi, zero_value) {
	int nb = kfs_atoi("0");

	cr_assert(nb == 0);
}

Test(kfs_atoi, not_numbers_returns_zero) {
	char* str_values[] = {"a", "abc1234", ")(42)", "trois"};
	int	  num_values[] = {0, 0, 0, 0};

	kfs_atoi_assert_many(str_values, num_values, 4);
}

Test(kfs_atoi, positive_values) {
	char* str_values[] = {"1", "+2", "+10", "42", "2147483647", "2147483648"};
	int	  num_values[] = {1, 2, 10, 42, 2147483647, -2147483648};

	kfs_atoi_assert_many(str_values, num_values, 6);
}

Test(kfs_atoi, negative_values) {
	char* str_values[] = {"-0", "-1", "-2", "-10", "-42", "-2147483648", "-2147483649"};
	int	  num_values[] = {0, -1, -2, -10, -42, -2147483648, 2147483647};

	kfs_atoi_assert_many(str_values, num_values, 7);
}

Test(kfs_atoi, nb_and_crap) {
	char* str_values[] = {"-0++", "-1?", "2\\4", "-10,craps12", "42.42", "21+21", "-2147483648{0}", "2147483647,iopp"};
	int	  num_values[] = {0, -1, 2, -10, 42, 21, -2147483648, 2147483647};

	kfs_atoi_assert_many(str_values, num_values, 8);
}

Test(kfs_atoi, mixed_signs_and_spaces) {
	char* str_values[] = {"--1", " +2", "    -10", "    ---42++", "+ 2147483647", "-+-+2147483647"};
	int	  num_values[] = {0, 2, -10, 0, 0, 0, 0};

	kfs_atoi_assert_many(str_values, num_values, 6);
}
