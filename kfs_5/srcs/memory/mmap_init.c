#include "kernel.h"
#include "mmap.h"
#include "mmap_inline.h"
#include "panic.h"

static void			   set_memory_size(mmap_t* mmap, uint8_t* start_addr, uint32_t size_kb);
static void			   set_mmap_addresses(mmap_t* mmap, uint8_t* start);
static inline uint8_t* get_remain_start(uint8_t* memory_start, uint32_t memory_size);
static uint32_t		   get_remain_len(uint32_t size);
static inline uint32_t round_up_memory_size(uint32_t size);
static inline bool	   valid_max_chunk_aligned_page_index(uint32_t page_index);
static uint32_t		   compute_max_shift(uint32_t bytes_nb);

void init_mmap(mmap_t* mmap, uint8_t* start, uint8_t* memory_start, uint32_t memory_size_kb) {
	set_memory_size(mmap, memory_start, memory_size_kb);
	set_mmap_addresses(mmap, start);
	set_all_memory_free(mmap);
	book_memory(mmap, get_remain_start(memory_start, memory_size_kb), get_remain_len(memory_size_kb), MMAP_UNAVAILABLE);
}

static void set_memory_size(mmap_t* mmap, uint8_t* start_addr, uint32_t size_kb) {
	mmap->start_index = get_page_index(start_addr);
	if (valid_max_chunk_aligned_page_index(mmap->start_index) == false) {
		panic("memory error: memory start must be 2^15 bytes aligned");
	}

	// 4 Kib per page
	uint32_t page_nb = round_up_memory_size(size_kb) >> 2;
	mmap->end_index = mmap->start_index + page_nb - 1;
	mmap->bytes_nb = page_nb >> PAGES_PER_BYTE_SHIFT;
	mmap->max_shift = compute_max_shift(mmap->bytes_nb);
	printk("pages from %u to %u\n", mmap->start_index, mmap->end_index);
}

static uint32_t compute_max_shift(uint32_t bytes_nb) {
	uint32_t max_shift = round_up_power_two(bytes_nb);
	if (max_shift > MMAP_MAX_SHIFT) {
		max_shift = MMAP_MAX_SHIFT;
	}
	return (max_shift);
}

static inline uint32_t round_up_memory_size(uint32_t size) {
	if (size % (1 << MMAP_MAX_SHIFT) != 0) {
		size /= (1 << MMAP_MAX_SHIFT);
		++size;
		size <<= MMAP_MAX_SHIFT;
	}
	return (size);
}

static inline bool valid_max_chunk_aligned_page_index(uint32_t page_index) {
	return ((page_index % (1 << MMAP_MAX_SHIFT) == 0) ? true : false);
}

static void set_mmap_addresses(mmap_t* mmap, uint8_t* start) {
	mmap->mmap[0] = start;
	uint32_t offset = get_bytes_nb(mmap, 0);

	for (uint32_t i = 1; i <= mmap->max_shift; ++i) {
		mmap->mmap[i] = mmap->mmap[i - 1] + offset;
		offset >>= 1;
	}
}

static inline uint8_t* get_remain_start(uint8_t* memory_start, uint32_t memory_size) {
	return (memory_start + memory_size * 1024 - 1);
}

static uint32_t get_remain_len(uint32_t size) {
	return (round_up_memory_size(size) - size);
}
