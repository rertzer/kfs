#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"

#include "kfs_bitmap.h"
#include "kfs_bitmap_test.h"

static size_t array_to_bitmap_index(size_t array_index);
static void	  kfs_bitmap_assert_next(size_t value, size_t expected);

static size_t array_to_bitmap_index(size_t array_index) {
	return (array_index * BITMAP_BITS_PER_ENTRY);
}

static void kfs_bitmap_assert_next(size_t value, size_t expected) {
	cr_assert(value == expected, "next: %zu, expected: %zu\n", value, expected);
}

Test(bitmap, after_erase_bitmap_bits_at_zero) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);
	for (size_t i = 0; i < BITMAP_TEST_SIZE_T_SIZE; ++i) {
		cr_assert(bitmap[i] == 0);
	}
}

Test(bitmap, after_set_all_bits_at_one) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);
	for (size_t i = 0; i < BITMAP_TEST_SIZE_T_SIZE; ++i) {
		cr_assert(bitmap[i] == SIZE_MAX);
	}
}

Test(bitmap, get_bitmap_value) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);

	size_t bitmap_index = array_to_bitmap_index(12);
	bitmap[12] = 42;  // 42 == 0101010

	cr_assert(get_bitmap_value(bitmap, bitmap_index) == 0);
	cr_assert(get_bitmap_value(bitmap, bitmap_index + 1) == 1);
}

Test(bitmap, set_bitmap_value) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);

	size_t bitmap_index = array_to_bitmap_index(12);

	set_bitmap_value(bitmap, bitmap_index, 1);
	cr_assert(get_bitmap_value(bitmap, bitmap_index) == 1);

	set_bitmap_value(bitmap, bitmap_index + 7, 1);
	cr_assert(get_bitmap_value(bitmap, bitmap_index + 7) == 1);

	set_bitmap_value(bitmap, bitmap_index + 7, 0);
	cr_assert(get_bitmap_value(bitmap, bitmap_index + 7) == 0);
}

Test(bitmap, get_next_bitmap_returns_next_zero_value_index) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);
	size_t last_bitmap = array_to_bitmap_index(12);

	cr_assert(get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap) == last_bitmap + 1);

	set_bitmap_value(bitmap, last_bitmap + 1, 1);
	cr_assert(get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap) == last_bitmap + 2);

	bitmap[12] = SIZE_MAX;
	size_t next = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	size_t array_entry_size = array_to_bitmap_index(1);
	size_t expected = last_bitmap + array_entry_size;
	kfs_bitmap_assert_next(next, expected);
}

Test(bitmap, get_next_in_full_bitmap_returns_entry_value) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);
	const size_t last_bitmap = array_to_bitmap_index(12);

	size_t next = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	kfs_bitmap_assert_next(next, last_bitmap);
}

Test(bitmap, get_next_long_range) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	const size_t last_bitmap = array_to_bitmap_index(12);
	size_t		 empty_offset = array_to_bitmap_index(30);
	set_bitmap_value(bitmap, empty_offset, 0);

	size_t next = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	kfs_bitmap_assert_next(next, empty_offset);
}

Test(bitmap, get_next_before_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	const size_t last_bitmap = array_to_bitmap_index(12);
	size_t		 empty_offset = array_to_bitmap_index(3);
	set_bitmap_value(bitmap, empty_offset, 0);

	size_t next = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	kfs_bitmap_assert_next(next, empty_offset);
}

Test(bitmap, get_next_right_before_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	const size_t offset = array_to_bitmap_index(12);

	const size_t last_bitmap = offset + 7;
	const size_t empty_offset = offset + 6;
	set_bitmap_value(bitmap, empty_offset, 0);

	const size_t next = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	kfs_bitmap_assert_next(next, empty_offset);
}

Test(bitmap, get_first_easy_forward) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);

	for (size_t i = 0; i < BITMAP_TEST_SIZE; ++i) {
		size_t result = get_first_bitmap(bitmap, BITMAP_TEST_SIZE);
		kfs_bitmap_assert_next(result, i);
		set_bitmap_value(bitmap, i, 1);
	}
	size_t result = get_first_bitmap(bitmap, BITMAP_TEST_SIZE);
	kfs_bitmap_assert_next(result, BITMAP_TEST_SIZE);
}

Test(bitmap, get_first_full) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	size_t result = get_first_bitmap(bitmap, BITMAP_TEST_SIZE);
	kfs_bitmap_assert_next(result, BITMAP_TEST_SIZE);
}

Test(bitmap, get_first_needle_in_hay_stack) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);
	set_bitmap_value(bitmap, 666, 0);
	set_bitmap_value(bitmap, 667, 0);

	size_t result = get_first_bitmap(bitmap, BITMAP_TEST_SIZE);
	kfs_bitmap_assert_next(result, 666);
}
