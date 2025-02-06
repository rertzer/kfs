#ifndef MMAP_INLINE_H
#define MMAP_INLINE_H

#include "mmap.h"

inline uint32_t get_bytes_nb(mmap_t* mmap, uint32_t size) {
	return (mmap->bytes_nb >> size);
}

inline uint8_t get_byte_status(uint8_t byte, uint8_t offset) {
	byte >>= offset;
	byte &= MMAP_PAGE_MASK;

	return (byte);
}

#endif
