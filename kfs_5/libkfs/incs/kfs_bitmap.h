#ifndef KFS_BITMAP_H
#define KFS_BITMAP_H

#include <stdbool.h>
#include <stddef.h>

#define BITMAP_BITS_PER_ENTRY (sizeof(size_t) * 8)
#define BITMAP_MAX_ENTRY_VALUE (size_t)-1

typedef struct {
	size_t array_offset;
	size_t array_value;
	size_t bit_mask;
	size_t bit_offset;
	size_t bit_value;
} bitmap_bit_t;

void   bitmap_erase(size_t* bitmap, size_t size);
void   bitmap_set_all(size_t* bitmap, size_t size);
bool   get_bitmap_value(size_t* bitmap, size_t offset);
void   set_bitmap_value(size_t* bitmap, size_t offset, bool value);
size_t get_next_bitmap(size_t* bitmap, size_t size, const size_t offset);
size_t get_first_bitmap(size_t* bitmap, size_t size);

#endif
