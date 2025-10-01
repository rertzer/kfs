#include "kfs_bitmap.h"
#include <stdio.h>

void bitmap_erase(size_t* bitmap, size_t size) {
	size /= sizeof(size_t);
	for (size_t i = 0; i < size; ++i) {
		bitmap[i] = 0;
	}
}

bool get_bitmap_value(size_t* bitmap, size_t offset) {
	size_t array_offset = offset / (sizeof(size_t) * 8);
	size_t value_offset = offset % (sizeof(size_t) * 8);
	size_t mask = 1 << value_offset;
	size_t array_value = bitmap[array_offset];

	return (array_value & mask);
}

void set_bitmap_value(size_t* bitmap, size_t offset, bool value) {
	size_t array_offset = offset / (sizeof(size_t) * 8);
	size_t value_offset = offset % (sizeof(size_t) * 8);
	size_t array_value = bitmap[array_offset];

	size_t mask = ~(1 << value_offset);
	array_value &= mask;
	array_value |= value << value_offset;
	bitmap[array_offset] = array_value;
}

static size_t get_next_bit(size_t value, size_t offset) {
	for (size_t i = offset; i < sizeof(size_t) * 8; ++i) {
		if ((value & (1 << i)) == 0) {
			return (i);
		}
	}
	return (sizeof(size_t) * 8);
}

size_t get_next_bitmap(size_t* bitmap, size_t size, size_t offset) {
	size_t array_offset = offset / (sizeof(size_t) * 8);
	size_t value_offset = offset % (sizeof(size_t) * 8);
	size_t array_value = bitmap[array_offset];
	size_t bit_offset = get_next_bit(array_value, value_offset + 1);
	if (bit_offset != sizeof(size_t) * 8) {
		return (array_offset * sizeof(size_t) * 8 + bit_offset);
	}

	for (++array_offset; array_offset < size; ++array_offset) {
		array_value = bitmap[array_offset];
		size_t bit_offset = get_next_bit(array_value, 0);
		if (bit_offset != sizeof(size_t) * 8) {
			return (array_offset * sizeof(size_t) * 8 + bit_offset);
		}
	}
	return (offset);
}
