#include "kfs_bitmap.h"
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"
#include "kfs_bitmap_test.h"

Test(bitmap, erase) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);
	for (size_t i = 0; i < BITMAP_TEST_SIZE_T_SIZE; ++i) {
		cr_assert(bitmap[i] == 0);
	}
}

Test(bitmap, set_all) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);
	for (size_t i = 0; i < BITMAP_TEST_SIZE_T_SIZE; ++i) {
		cr_assert(bitmap[i] == SIZE_MAX);
	}
}
Test(bitmap, get_bitmap_value) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);
	bitmap[12] = 42;  // 42 == 0101010

	cr_assert(get_bitmap_value(bitmap, 12 * 8 * sizeof(size_t)) == 0);
	cr_assert(get_bitmap_value(bitmap, 12 * 8 * sizeof(size_t) + 1) == 1);
}

Test(bitmap, set_bitmap_value) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);

	set_bitmap_value(bitmap, 12 * 8 * sizeof(size_t), 1);
	cr_assert(get_bitmap_value(bitmap, 12 * 8 * sizeof(size_t)) == 1);

	set_bitmap_value(bitmap, 12 * 8 * sizeof(size_t) + 7, 1);
	cr_assert(get_bitmap_value(bitmap, 12 * 8 * sizeof(size_t) + 7) == 1);

	set_bitmap_value(bitmap, 12 * 8 * sizeof(size_t) + 7, 0);
	cr_assert(get_bitmap_value(bitmap, 12 * 8 * sizeof(size_t) + 7) == 0);
}

Test(bitmap, get_next_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_erase(bitmap, BITMAP_TEST_SIZE);
	size_t last_bitmap = 12 * 8 * sizeof(size_t);

	cr_assert(get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap) == last_bitmap + 1);

	set_bitmap_value(bitmap, last_bitmap + 1, 1);
	cr_assert(get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap) == last_bitmap + 2);
	bitmap[12] = SIZE_MAX;
	size_t got = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	size_t expected = last_bitmap + sizeof(size_t) * 8;
	cr_assert(got == expected, "%zu %zu\n", got, expected);
}

Test(bitmap, get_full_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);
	const size_t last_bitmap = 12 * 8 * sizeof(size_t);

	size_t got = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	cr_assert(got == last_bitmap, "%zu %zu\n", got, last_bitmap);
}

Test(bitmap, get_next_far_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	const size_t last_bitmap = 12 * BITMAP_BITS_PER_ENTRY;
	size_t		 empty_offset = 30 * BITMAP_BITS_PER_ENTRY;
	set_bitmap_value(bitmap, empty_offset, 0);

	size_t get = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	cr_assert(get == empty_offset, "%zu %zu\n", get, empty_offset);
}

Test(bitmap, get_next_before_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	const size_t last_bitmap = 12 * BITMAP_BITS_PER_ENTRY;
	size_t		 empty_offset = 3 * BITMAP_BITS_PER_ENTRY;
	set_bitmap_value(bitmap, empty_offset, 0);

	size_t get = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	cr_assert(get == empty_offset, "%zu %zu\n", get, empty_offset);
}

Test(bitmap, get_right_before_bitmap) {
	size_t bitmap[BITMAP_TEST_SIZE_T_SIZE];
	bitmap_set_all(bitmap, BITMAP_TEST_SIZE);

	const size_t last_bitmap = 12 * BITMAP_BITS_PER_ENTRY + 7;
	size_t		 empty_offset = 12 * BITMAP_BITS_PER_ENTRY + 6;
	set_bitmap_value(bitmap, empty_offset, 0);

	size_t get = get_next_bitmap(bitmap, BITMAP_TEST_SIZE, last_bitmap);
	cr_assert(get == empty_offset, "%zu %zu\n", get, empty_offset);
}
