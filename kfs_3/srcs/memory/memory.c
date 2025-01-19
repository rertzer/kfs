#include "memory.h"
#include "boot_infos.h"
#include "kernel.h"
#include "mmap.h"

extern uint32_t p_mmap_start;

mmap_t p_mmap;

static void*	memory_adder(void* addr, uint32_t len);
static bool		memory_overflow(void* addr, uint32_t len);
static void		freeze_pages(uint32_t start_page_index, uint32_t end_page_index);
static void		freeze_page(uint32_t page_index, uint32_t size);
static uint32_t get_page_range_chunk_size(uint32_t start_page_index, uint32_t end_page_index);

void init_memory() {
	init_mmap(&p_mmap, (uint8_t*)&p_mmap_start);
	boot_infos_memory_map_freeze();
}

void freeze_memory(uint8_t* addr, uint32_t len) {
	uint32_t start_page_index = get_page_index(addr);

	uint32_t end_page_index = get_page_index(memory_adder(addr, len));
	freeze_pages(start_page_index, end_page_index);
}

uint32_t get_page_index(void* addr) {
	return ((uint32_t)addr >> 12);
}

static void freeze_pages(uint32_t start_page_index, uint32_t end_page_index) {
	uint32_t size = get_page_range_chunk_size(start_page_index, end_page_index);
	freeze_page(start_page_index, size);
	start_page_index += 1 << size;
	if (start_page_index <= end_page_index) {
		freeze_pages(start_page_index, end_page_index);
	}
}

static uint32_t get_page_range_chunk_size(uint32_t start_page_index, uint32_t end_page_index) {
	uint32_t size = get_start_max_possible_chunk_size(start_page_index);
	uint32_t len = get_len_max_possible_chunk_size(end_page_index - start_page_index + 1);
	size = size < len ? size : len;
	return (size);
}

static void* memory_adder(void* addr, uint32_t len) {
	uint32_t sum;
	if (memory_overflow(addr, len)) {
		sum = UINT32_MAX;
	} else {
		sum = (uint32_t)addr + len - 1;
	}
	return ((void*)sum);
}

static bool memory_overflow(void* addr, uint32_t len) {
	bool over = false;
	if (len > UINT32_MAX - (uint32_t)addr) {
		over = true;
	}
	return (over);
}

static void freeze_page(uint32_t page_index, uint32_t size) {
	chunk_t chunk = get_chunk(&p_mmap, page_index);
	if (chunk.status == MMAP_FREEZED) {
		return;
	}
	if (chunk.status == MMAP_FREE) {
		if (chunk.size == size) {
			chunk.status = MMAP_FREEZED;
			set_chunk_status(&p_mmap, chunk);
		} else {
			split_chunk(&p_mmap, chunk);
			freeze_page(page_index, size);
		}
	}
}
