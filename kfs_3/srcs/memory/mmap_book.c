#include "kernel.h"
#include "mmap.h"
#include "mmap_inline.h"

#include "utils.h" //for getting min

static void			   book_pages(mmap_t* mmap, uint32_t start_page_index, uint32_t end_page_index, uint32_t status);
static void			   book_page(mmap_t* mmap, uint32_t page_index, uint32_t shift, uint32_t status);
static void*		   memory_adder(void const* const addr, uint32_t const len);
static inline bool	   memory_overflow(void const* const addr, uint32_t const len);
static uint32_t		   get_page_range_shift(uint32_t const max_shift,
											uint32_t const start_page_index,
											uint32_t const end_page_index);
static inline uint32_t get_start_max_shift(uint32_t const max_shift, uint32_t const page_index);
static uint32_t		   get_len_max_shift(uint32_t const max_shift, uint32_t const len);
static inline bool	   max_len(uint32_t const max_shift, uint32_t const len);
static inline bool	   bit_is_set(uint32_t const map, uint32_t const offset);
static inline uint32_t shift_start_index(uint32_t const start_page_index, uint32_t const shift);
static inline bool	   status_already_set(uint32_t chunk_status, uint32_t status);
static inline bool	   splited_area(uint32_t chunk_shift, uint32_t shift);
static void			   book_kids(mmap_t* mmap, uint32_t page_index, uint32_t shift, uint32_t status);
static inline uint32_t right_buddy_page_index(uint32_t page_index, uint32_t shift);

void book_memory(mmap_t* mmap, uint8_t* addr, uint32_t len, uint32_t status) {
	if (len == 0) {
		return;
	}
	uint32_t start_page_index = get_page_index(addr);
	uint32_t end_page_index = get_page_index(memory_adder(addr, len));
	book_pages(mmap, start_page_index, end_page_index, status);
}

static void* memory_adder(void const* const addr, uint32_t const len) {
	return ((void*)((memory_overflow(addr, len)) ? UINT32_MAX : (uint32_t)addr + len - 1));
}

static inline bool memory_overflow(void const* const addr, uint32_t const len) {
	return ((len > UINT32_MAX - (uint32_t)addr) ? true : false);
}

static void book_pages(mmap_t* mmap, uint32_t start_page_index, uint32_t end_page_index, uint32_t status) {
	uint32_t shift = get_page_range_shift(mmap->max_shift, start_page_index, end_page_index);
	book_page(mmap, start_page_index, shift, status);
	start_page_index = shift_start_index(start_page_index, shift);
	if (start_page_index <= end_page_index) {
		book_pages(mmap, start_page_index, end_page_index, status);
	}
}

static uint32_t get_page_range_shift(uint32_t const max_shift,
									 uint32_t const start_page_index,
									 uint32_t const end_page_index) {
	uint32_t const start_shift = get_start_max_shift(max_shift, start_page_index);
	uint32_t const len_shift = get_len_max_shift(max_shift, index_len(start_page_index, end_page_index));
	return (min(start_shift, len_shift));
}

static inline uint32_t shift_start_index(uint32_t const start_page_index, uint32_t const shift) {
	return ((start_page_index + 1) << shift);
}

static inline uint32_t get_start_max_shift(uint32_t const max_shift, uint32_t const page_index) {
	uint32_t max = 0;

	for (; max < max_shift; ++max) {
		if (bit_is_set(page_index, max)) {
			break;
		}
	}
	return (max);
}

static uint32_t get_len_max_shift(uint32_t const max_shift, uint32_t const len) {
	uint32_t max = max_shift - 1;

	if (max_len(max_shift, len)) {
		return (max_shift);
	}
	for (; max > 0; --max) {
		if (bit_is_set(len, max)) {
			break;
		}
	}
	return (max);
}

static inline bool max_len(uint32_t const max_shift, uint32_t const len) {
	return (len >= ((uint32_t)1 << max_shift));
}

static inline bool bit_is_set(uint32_t const map, uint32_t const offset) {
	return ((bool)((map & ((uint32_t)1 << offset)) != 0));
}

static void book_page(mmap_t* mmap, uint32_t page_index, uint32_t shift, uint32_t status) {
	if (valid_page_index(mmap, page_index) == false) {
		return;
	}
	chunk_t chunk = get_chunk(mmap, page_index);
	if (status_already_set(chunk.status, status)) {
		return;
	}
	if (splited_area(chunk.shift, shift)) {
		book_kids(mmap, page_index, shift, status);
	} else if (chunk.status == MMAP_FREE) {
		if (chunk.shift == shift) {
			set_chunk_status(mmap, &chunk, status);
		} else {
			split_chunk(mmap, chunk);
			book_page(mmap, page_index, shift, status);
		}
	}
}

static inline bool status_already_set(uint32_t chunk_status, uint32_t status) {
	return (chunk_status == status && status != MMAP_UNAVAILABLE);
}

static inline bool splited_area(uint32_t chunk_shift, uint32_t shift) {
	return (chunk_shift < shift);
}

static void book_kids(mmap_t* mmap, uint32_t page_index, uint32_t shift, uint32_t status) {
	shift -= 1;
	book_page(mmap, page_index, shift, status);
	book_page(mmap, right_buddy_page_index(page_index, shift), shift, status);
}

static inline uint32_t right_buddy_page_index(uint32_t page_index, uint32_t shift) {
	return (page_index + (1 << shift));
}
