#include "mmap.h"
#include "memory.h"
#include "mmap_inline.h"
#include "panic.h"

static inline void*	  get_address_by_local_page_index(mmap_t* mmap, uint32_t local_page_index);
static inline uint8_t set_byte_status(uint8_t byte, chunk_t chunk);
static chunk_t		  get_chunk_by_shift(mmap_t* mmap, uint32_t page_index, uint32_t shift);
static void			  split_chunk_set_parent(mmap_t* mmap, chunk_t chunk);
static void			  split_chunk_set_left_kid(mmap_t* mmap, chunk_t* chunk);
static void			  split_chunk_set_right_kid(mmap_t* mmap, chunk_t chunk);

/* =============================== set all memory free ====================== */

void set_all_memory_free(mmap_t* mmap) {
	uint32_t size_bytes_len = get_bytes_nb(mmap, mmap->max_shift);
	for (uint32_t byte_index = 0; byte_index < size_bytes_len; ++byte_index) {
		mmap->mmap[mmap->max_shift][byte_index] = 0xFF;
	}
}

/* =============================== set chunk status ========================= */

void set_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint8_t byte = mmap->mmap[chunk.shift][chunk.byte];
	byte = set_byte_status(byte, chunk);
	mmap->mmap[chunk.shift][chunk.byte] = byte;
}

static inline uint8_t set_byte_status(uint8_t byte, chunk_t chunk) {
	byte &= ~(MMAP_PAGE_MASK << chunk.offset);
	byte |= chunk.status << chunk.offset;

	return (byte);
}

/* =============================== get chunk status ========================= */

uint32_t get_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint32_t byte = mmap->mmap[chunk.shift][chunk.byte];
	return (get_byte_status(byte, chunk.offset));
}

/* =============================== get chunk  =============================== */

chunk_t get_chunk(mmap_t* mmap, uint32_t page_index) {
	chunk_t chunk;

	if (valid_page_index(mmap, page_index) == false) {
		panic("error: invalid page index");
	}
	for (uint32_t shift = mmap->max_shift; shift != UINT32_MAX; --shift) {
		chunk = get_chunk_by_shift(mmap, page_index, shift);
		if (chunk.status != MMAP_UNAVAILABLE) {
			break;
		}
	}
	return (chunk);
}

static chunk_t get_chunk_by_shift(mmap_t* mmap, uint32_t page_index, uint32_t shift) {
	chunk_t	 chunk;
	uint32_t chunk_index = get_local_page_index(mmap, page_index) >> shift;

	chunk.shift = shift;
	chunk.byte = get_byte(chunk_index);
	chunk.offset = get_offset(chunk_index);
	chunk.status = get_byte_status(mmap->mmap[chunk.shift][chunk.byte], chunk.offset);
	return (chunk);
}

/* =============================== get size by address ====================== */

uint32_t get_size_by_address(mmap_t* mmap, void const* const addr) {
	uint32_t size = 0;
	chunk_t	 chunk = get_chunk(mmap, get_page_index(addr));

	if (chunk.status == MMAP_FREE || chunk.status == MMAP_USED || chunk.status == MMAP_USED_RONLY) {
		size = get_byte_size(chunk.shift);
	}
	return (size);
}

/* =============================== get mmap info ============================ */

chunk_info_t get_chunk_info(mmap_t* mmap, void const* const addr) {
	chunk_info_t chunk_info;
	chunk_t		 chunk = get_chunk(mmap, get_page_index(addr));

	chunk_info.addr = addr;
	chunk_info.page_nb = get_page_size(chunk.shift);
	chunk_info.shift = chunk.shift;
	chunk_info.status = chunk.status;
	chunk_info.valid = (chunk.status == MMAP_UNAVAILABLE) ? false : true;

	return (chunk_info);
}

/* =============================== get chunk address  ======================= */

void* get_chunk_address(mmap_t* mmap, chunk_t chunk) {
	uint32_t local_page_index = get_chunk_index(chunk) << chunk.shift;
	return (get_address_by_local_page_index(mmap, local_page_index));
}

static inline void* get_address_by_local_page_index(mmap_t* mmap, uint32_t local_page_index) {
	return ((void*)((local_page_index + mmap->start_index) * PAGE_SIZE));
}

/* =============================== split chunk ============================== */

void split_chunk(mmap_t* mmap, chunk_t chunk) {
	if (chunk.shift == 0 || chunk.status != MMAP_FREE) {
		return;
	}
	split_chunk_set_parent(mmap, chunk);
	split_chunk_set_left_kid(mmap, &chunk);
	split_chunk_set_right_kid(mmap, chunk);
}

static void split_chunk_set_parent(mmap_t* mmap, chunk_t chunk) {
	chunk.status = MMAP_UNAVAILABLE;
	set_chunk_status(mmap, chunk);
}

static void split_chunk_set_left_kid(mmap_t* mmap, chunk_t* chunk) {
	// twice more entries one level lower
	uint32_t index = get_chunk_index(*chunk) << 1;

	chunk->shift -= 1;
	chunk->byte = get_byte(index);
	chunk->offset = get_offset(index);
	chunk->status = MMAP_FREE;
	set_chunk_status(mmap, *chunk);
}

static void split_chunk_set_right_kid(mmap_t* mmap, chunk_t chunk) {
	chunk.offset += BITS_PER_CHUNK;
	set_chunk_status(mmap, chunk);
}
