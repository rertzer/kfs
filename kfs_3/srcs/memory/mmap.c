#include "mmap.h"
#include "kernel.h"

static inline uint32_t get_byte(uint32_t chunk_index);
static inline uint32_t get_offset(uint32_t chunk_index);
static uint8_t		   set_byte_status(uint8_t byte, chunk_t chunk);
static uint8_t		   get_byte_status(uint8_t byte, uint8_t offset);
static void			   set_chunks_lists_pointers(mmap_t* mmap, uint8_t* start);
static void			   set_big_chunks_as_free(mmap_t* mmap);

static inline uint32_t get_byte(uint32_t chunk_index) {
	return (chunk_index / PAGES_PER_BYTE);
}

static inline uint32_t get_offset(uint32_t chunk_index) {
	return ((chunk_index % PAGES_PER_BYTE) << 1);
}

static uint8_t set_byte_status(uint8_t byte, chunk_t chunk) {
	byte &= ~(MMAP_PAGE_MASK << chunk.offset);
	byte |= chunk.status << chunk.offset;

	return (byte);
}

static uint8_t get_byte_status(uint8_t byte, uint8_t offset) {
	byte >>= offset;
	byte &= MMAP_PAGE_MASK;

	return (byte);
}

uint32_t get_chunk_index(chunk_t chunk) {
	return (chunk.byte * PAGES_PER_BYTE + (chunk.offset >> BITS_PER_PAGE_SHIFT));
}

void init_mmap(mmap_t* mmap, uint8_t* start) {
	set_chunks_lists_pointers(mmap, start);
	set_big_chunks_as_free(mmap);
}

static void set_chunks_lists_pointers(mmap_t* mmap, uint8_t* start) {
	(*mmap)[0] = start;
	uint32_t offset = ONE_PAGE_BYTES_NB;

	// printk("map 0 %u %08x\n", offset, (*mmap)[0]);
	for (uint32_t i = 1; i <= MMAP_MAX_SIZE; ++i) {
		(*mmap)[i] = (*mmap)[i - 1] + offset;
		// printk("map %u %u %08x\n", i, offset, (*mmap)[i]);
		offset >>= 1;
	}
}

static void set_big_chunks_as_free(mmap_t* mmap) {
	for (uint32_t i = 0; i < MAX_SIZE_PAGE_BYTES_NB; ++i) {
		(*mmap)[MMAP_MAX_SIZE][i] = 0xFF;
	}
}

// typedef struct chunk {
// 	uint32_t size;
// 	uint32_t byte;
// 	uint32_t offset;
// 	uint32_t status;
// } chunk_t;
//
chunk_t make_chunk(uint32_t size, uint32_t chunk_index, uint32_t status) {
	chunk_t chunk;
	chunk.size = size;
	chunk.byte = get_byte(chunk_index);
	chunk.offset = get_offset(chunk_index);
	chunk.status = status;

	return (chunk);
}

chunk_t get_chunk(mmap_t* mmap, uint32_t page_index) {
	chunk_t chunk;

	for (chunk.size = MMAP_MAX_SIZE; chunk.size != UINT32_MAX; --chunk.size) {
		uint32_t current_index = page_index >> chunk.size;
		chunk.byte = get_byte(current_index);
		chunk.offset = get_offset(current_index);
		uint32_t byte = (*mmap)[chunk.size][chunk.byte];
		chunk.status = get_byte_status(byte, chunk.offset);
		if (chunk.status != MMAP_UNAVAILABLE) {
			break;
		}
	}
	return (chunk);
}
void set_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint8_t byte = (*mmap)[chunk.size][chunk.byte];
	byte = set_byte_status(byte, chunk);
	(*mmap)[chunk.size][chunk.byte] = byte;
}

void split_chunk(mmap_t* mmap, chunk_t);

uint32_t get_start_max_possible_chunk_size(uint32_t page_index) {
	uint32_t max = 0;

	for (; max < MMAP_MAX_SIZE; ++max) {
		if ((page_index & 1) == 1) {
			break;
		}
		page_index >>= 1;
	}

	return (max);
}

uint32_t get_len_max_possible_chunk_size(uint32_t len) {
	uint32_t max = MMAP_MAX_SIZE - 1;

	if (len >= (1 << MMAP_MAX_SIZE)) {
		return (MMAP_MAX_SIZE);
	}
	for (; max > 0; --max) {
		if ((len & (1 << (max))) != 0) {
			break;
		}
	}

	return (max);
}
void split_chunk(mmap_t* mmap, chunk_t chunk) {
	if (chunk.size == 0) {
		return;
	}
	uint32_t index = get_chunk_index(chunk);
	// twice more entries one level lower
	index <<= 1;

	chunk_t kid_chunk;
	// left child
	kid_chunk.size = chunk.size - 1;
	kid_chunk.byte = get_byte(index);
	kid_chunk.offset = get_offset(index);
	kid_chunk.status = MMAP_FREE;
	set_chunk_status(mmap, kid_chunk);
	// right child
	kid_chunk.offset += BITS_PER_CHUNK;
	set_chunk_status(mmap, kid_chunk);
	// parent
	chunk.status = MMAP_UNAVAILABLE;
	set_chunk_status(mmap, chunk);
}
