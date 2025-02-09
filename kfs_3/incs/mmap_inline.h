#ifndef MMAP_INLINE_H
#define MMAP_INLINE_H

#include "memory.h"
#include "mmap.h"

inline uint32_t get_bytes_nb(mmap_t* mmap, uint32_t shift) {
	return (mmap->bytes_nb >> shift);
}

inline uint8_t get_byte_status(uint8_t byte, uint8_t offset) {
	byte >>= offset;
	byte &= MMAP_PAGE_MASK;

	return (byte);
}

static inline uint32_t get_local_page_index(mmap_t* mmap, uint32_t page_index) {
	return (page_index - mmap->start_index);
}

static inline bool valid_page_index(mmap_t* mmap, uint32_t page_index) {
	bool valid = true;
	if (page_index < mmap->start_index || page_index > mmap->end_index) {
		valid = false;
	}
	return (valid);
}

static inline uint32_t get_byte(uint32_t chunk_index) {
	return (chunk_index / PAGES_PER_BYTE);
}

static inline uint32_t get_page_size(uint32_t chunk_size) {
	return (1 << chunk_size);
}

static inline uint32_t get_byte_size(uint32_t chunk_size) {
	return ((1 << chunk_size) * PAGE_SIZE);
}

static inline uint32_t get_offset(uint32_t chunk_index) {
	return ((chunk_index % PAGES_PER_BYTE) << 1);
}

static inline uint32_t get_chunk_index(chunk_t chunk) {
	return (chunk.byte * PAGES_PER_BYTE + (chunk.offset >> BITS_PER_PAGE_SHIFT));
}

static inline uint32_t get_page_index(void* const addr) {
	return ((uint32_t)addr >> 12);
}
#endif
