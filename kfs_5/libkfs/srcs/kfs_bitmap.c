#include "kfs_bitmap.h"
#include <stdint.h>
#include <stdio.h>

static bitmap_bit_t get_bitmap_bit(size_t* bitmap, size_t offset);
static size_t		get_next_in_range(size_t* bitmap, size_t start_index, size_t end_index);

void bitmap_erase(size_t* bitmap, size_t size) {
	size /= BITMAP_BITS_PER_ENTRY;
	for (size_t i = 0; i < size; ++i) {
		bitmap[i] = 0;
	}
}

void bitmap_set_all(size_t* bitmap, size_t size) {
	size /= BITMAP_BITS_PER_ENTRY;
	for (size_t i = 0; i < size; ++i) {
		bitmap[i] = SIZE_MAX;
	}
}

bool get_bitmap_value(size_t* bitmap, size_t offset) {
	bitmap_bit_t bb = get_bitmap_bit(bitmap, offset);
	return (bb.bit_value);
}

void set_bitmap_value(size_t* bitmap, size_t offset, bool value) {
	bitmap_bit_t bb = get_bitmap_bit(bitmap, offset);
	bb.array_value &= ~(bb.bit_mask);
	bb.array_value |= value << bb.bit_offset;
	bitmap[bb.array_offset] = bb.array_value;
}

static size_t get_next_bit(size_t value, size_t offset) {
	for (size_t i = offset; i < BITMAP_BITS_PER_ENTRY; ++i) {
		if ((value & ((size_t)1 << i)) == 0) {
			return (i);
		}
	}
	return (BITMAP_BITS_PER_ENTRY);
}

/* size in bits
 * if table full, return the offset given as argument
 */

size_t get_next_bitmap(size_t* bitmap, size_t size, size_t const offset) {
	bitmap_bit_t current = get_bitmap_bit(bitmap, offset);
	current.bit_offset = get_next_bit(current.array_value, current.bit_offset + 1);
	size_t next_offset = offset;
	if (current.bit_offset != BITMAP_BITS_PER_ENTRY) {
		next_offset = current.array_offset * BITMAP_BITS_PER_ENTRY + current.bit_offset;
		return (next_offset);
	}
	next_offset = get_next_in_range(bitmap, offset, size);
	if (next_offset != offset) {
		return (next_offset);
	}
	next_offset = get_next_in_range(bitmap, SIZE_MAX, offset + BITMAP_BITS_PER_ENTRY);
	if (next_offset != SIZE_MAX) {
		return (next_offset);
	}

	return (offset);
}

static bitmap_bit_t get_bitmap_bit(size_t* bitmap, size_t offset) {
	bitmap_bit_t bb;
	bb.array_offset = offset / BITMAP_BITS_PER_ENTRY;
	bb.array_value = bitmap[bb.array_offset];
	bb.bit_offset = offset % BITMAP_BITS_PER_ENTRY;
	bb.bit_mask = 1 << bb.bit_offset;
	bb.bit_value = bb.array_value & bb.bit_mask;
	return (bb);
}

static size_t get_next_in_range(size_t* bitmap, size_t start_index, size_t end_index) {
	size_t not_found = start_index;
	++start_index;
	start_index /= BITMAP_BITS_PER_ENTRY;
	end_index /= BITMAP_BITS_PER_ENTRY;
	for (size_t index = start_index; index < end_index; ++index) {
		size_t array_value = bitmap[index];
		size_t bit_offset = get_next_bit(array_value, 0);
		if (bit_offset != BITMAP_BITS_PER_ENTRY) {
			return (index * BITMAP_BITS_PER_ENTRY + bit_offset);
		}
	}
	return (not_found);
}
