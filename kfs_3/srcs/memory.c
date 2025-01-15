#include "memory.h"
#include "boot_infos.h"
#include "kernel.h"

extern uint32_t p_mmap_start;

volatile uint8_t* p_mmap[16];

static void	   create_p_mmap();
static void*   memory_adder(void* addr, uint32_t offset);
static bool	   memory_overflow(void* addr, uint32_t len);
static void	   freeze_page(uint32_t page_index);
static uint8_t get_block_status(uint32_t block_size, uint32_t block_index);
static void	   split_block(uint32_t block_size, uint32_t block_index);
static void	   set_block_status(uint32_t block_size, uint32_t block_index, uint8_t block_status);

void init_memory() {
	create_p_mmap();
	memory_map_freeze();
}

static void create_p_mmap() {
	p_mmap[0] = (uint8_t*)&p_mmap_start;
	// 4 Gb give 1048576 pages, bitmap hold 4 pages per byte, gives 262144 bytes to hold 1 page
	// blocks in the bitmap
	uint32_t offset = 262144;

	printk("map 0 %u %08x\n", offset, p_mmap[0]);
	for (uint32_t i = 1; i < 16; ++i) {
		p_mmap[i] = p_mmap[i - 1] + offset;
		printk("map %u %u %08x\n", i, offset, p_mmap[i]);
		offset >>= 1;
	}
	// higher blocks size 2^15 available
	for (uint32_t i = 0; i < 8; ++i) {
		p_mmap[15][i] = 0xFF;
	}
}

void freeze_memory(uint8_t* addr, uint32_t len) {
	uint32_t start_page_index = get_page_index(addr);

	uint32_t end_page_index = get_page_index(memory_adder(addr, len));
	printk("page index %u\n", start_page_index);
	printk("page index %u\n", end_page_index);
	for (uint32_t page_index = start_page_index; page_index <= end_page_index; ++page_index) {
		freeze_page(page_index);
	}
}

uint32_t get_page_index(void* addr) {
	return ((uint32_t)addr >> 12);
}

static void* memory_adder(void* addr, uint32_t offset) {
	uint32_t sum;
	if (memory_overflow(addr, offset)) {
		sum = UINT32_MAX;
	} else {
		sum = (uint32_t)addr + offset;
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

static void freeze_page(uint32_t page_index) {
	// going down block sizes
	for (uint32_t i = 15; i > 0; ++i) {
		// get block start
		uint32_t block_start_index = page_index << i;

		if (get_block_status(i, block_start_index) == MMAP_FREE) {
			split_block(i, block_start_index);
		}
		// if block free and size not 1 split
	}
	//
	// once block size 1 write as freezed
}

static uint8_t get_block_status(uint32_t block_size, uint32_t block_index) {
	uint32_t byte_index = block_index / PAGES_PER_BYTE;
	uint32_t byte_offset = (block_index % PAGES_PER_BYTE) << 1;
	uint8_t	 byte_block = p_mmap[block_size][byte_index];
	return ((byte_block >> byte_offset) & MMAP_PAGE_MASK);
}

static void set_block_status(uint32_t block_size, uint32_t block_index, uint8_t block_status) {
	uint32_t byte_index = block_index / PAGES_PER_BYTE;
	uint32_t byte_offset = (block_index % PAGES_PER_BYTE) << 1;
	uint8_t	 byte_block = p_mmap[block_size][byte_index];
	// erase previous block value
	byte_block &= ~(MMAP_PAGE_MASK << block_index);
	// add new status
	byte_block &= block_status << byte_offset;
	p_mmap[block_size][byte_index] = byte_block;
}

static void split_block(uint32_t block_size, uint32_t block_index) {}
