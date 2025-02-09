#include "mmap.h"
#include "mmap_inline.h"

static inline void get_unavailable_chunk(chunk_t* chunk);
static chunk_t	   get_free_chunk_by_shift(mmap_t* mmap, uint32_t shift);
static uint32_t	   get_free_offset(uint8_t byte);

chunk_t get_free_chunk(mmap_t* mmap, uint32_t shift) {
	chunk_t chunk;
	if (shift > 15) {
		get_unavailable_chunk(&chunk);
		return (chunk);
	}
	chunk = get_free_chunk_by_shift(mmap, shift);
	if (chunk.status != MMAP_FREE) {
		chunk = get_free_chunk(mmap, shift + 1);

		split_chunk(mmap, chunk);
		chunk = get_free_chunk_by_shift(mmap, shift);
	}
	return (chunk);
}

static inline void get_unavailable_chunk(chunk_t* chunk) {
	chunk->shift = 0;
	chunk->byte = 0;
	chunk->offset = MMAP_NOT_FOUND_OFFSET;
	chunk->status = MMAP_UNAVAILABLE;
}

static chunk_t get_free_chunk_by_shift(mmap_t* mmap, uint32_t shift) {
	uint32_t len = get_bytes_nb(mmap, shift);

	chunk_t chunk;
	chunk.shift = shift;
	chunk.status = MMAP_UNAVAILABLE;

	for (uint32_t i = 0; i < len; ++i) {
		uint8_t	 byte = mmap->mmap[shift][i];
		uint32_t offset = get_free_offset(byte);
		if (offset != MMAP_NOT_FOUND_OFFSET) {
			chunk.byte = i;
			chunk.offset = offset;
			chunk.status = MMAP_FREE;
			break;
		}
	}
	return (chunk);
}

static uint32_t get_free_offset(uint8_t byte) {
	uint32_t offset;

	for (offset = 0; offset < 8; offset += 2) {
		uint32_t status = get_byte_status(byte, offset);
		if (status == MMAP_FREE) {
			break;
		}
	}
	return (offset);
}
