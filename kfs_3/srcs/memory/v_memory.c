#include "kernel.h"
#include "memory.h"
#include "mmap.h"
#include "paging.h"

extern uint32_t const user_v_mmap_start;
extern uint32_t const user_v_mmap_end;
extern uint32_t const kernel_v_mmap_start;
extern uint32_t const kernel_v_mmap_end;

mmap_t user_virt_mmap;
mmap_t kernel_virt_mmap;

static inline uint8_t get_rw_status(bool const rw);
static inline mmap_t* get_v_mmap_by_address(void const* const addr);
static inline mmap_t* get_virtual_map(bool const level);
static inline bool	  get_address_level(void const* const addr);
static inline bool	  get_chunk_rw(uint32_t const status);
static inline bool	  get_mmap_info_valid(uint32_t const status,
										  bool const	 fault_rw,
										  bool const	 fault_level,
										  bool const	 mmap_level);

void init_v_memory() {
	printk("- virtual kernel memory size is 1 Gb\t");
	init_mmap(&user_virt_mmap, (uint8_t*)&user_v_mmap_start, (uint8_t*)USER_VIRTUAL_MEMORY_START,
			  USER_VIRTUAL_MEMORY_KIB_SIZE);
	printk("- virtual user memory size is 3 Gb\t");
	init_mmap(&kernel_virt_mmap, (uint8_t*)&kernel_v_mmap_start, (uint8_t*)KERNEL_VIRTUAL_MEMORY_START,
			  KERNEL_VIRTUAL_MEMORY_KIB_SIZE);
	book_memory(&kernel_virt_mmap, (uint8_t*)KERNEL_VIRTUAL_MEMORY_START, KERNEL_SIZE, MMAP_USED);
	book_memory(&kernel_virt_mmap, (uint8_t*)PAGE_DIR_BASE, 1024 * PAGE_SIZE, MMAP_USED);
	book_memory(&user_virt_mmap, (uint8_t*)USER_VIRTUAL_MEMORY_START, PAGE_SIZE, MMAP_UNAVAILABLE);
}

void* v_mmap(uint32_t size, bool const level, bool const rw) {
	mmap_t* const mmap = get_virtual_map(level);

	size = round_up_power_two(size / PAGE_SIZE);
	if (size > MMAP_MAX_SHIFT) {
		printk("v_mmap error: max size allowed is 2^15\n");
		return (NULL);
	}

	chunk_t chunk = get_free_chunk(mmap, size);
	if (chunk.status != MMAP_FREE) {
		return (NULL);
	}

	set_chunk_status(mmap, &chunk, get_rw_status(rw));
	return (get_chunk_address(mmap, chunk));
}

static inline mmap_t* get_virtual_map(bool const level) {
	mmap_t* mmap = &user_virt_mmap;

	if (level == SUPERVISOR_LEVEL) {
		mmap = &kernel_virt_mmap;
	}
	return (mmap);
}

static inline bool get_address_level(void const* const addr) {
	bool level = SUPERVISOR_LEVEL;

	if (addr < (void*)KERNEL_VIRTUAL_MEMORY_START) {
		level = USER_LEVEL;
	}
	return (level);
}

static inline uint8_t get_rw_status(bool const rw) {
	return ((uint8_t)(rw == READ_WRITE) ? MMAP_USED : MMAP_USED_RONLY);
}

uint32_t v_size(void const* const addr) {
	mmap_t const* const mmap = get_v_mmap_by_address(addr);
	return (get_size_by_address(mmap, addr));
}

uint8_t v_free(void const* const v_addr) {
	mmap_t* const mmap = get_v_mmap_by_address(v_addr);
	return (free_by_address(mmap, v_addr));
}

static inline mmap_t* get_v_mmap_by_address(void const* const addr) {
	mmap_t* mmap = &user_virt_mmap;
	if (addr >= (void*)((uint32_t)KERNEL_VIRTUAL_MEMORY_KIB_SIZE * 1024)) {
		mmap = &kernel_virt_mmap;
	}
	return (mmap);
}

void get_virtual_memory_infos(mem_info_t* const mem_infos, bool const level) {
	mmap_t const* const mmap = get_virtual_map(level);
	get_mmap_infos(mmap, mem_infos);
}

mmap_info_t v_mmap_check(void const* const l_address, bool const fault_level, bool const fault_rw) {
	bool const		   address_level = get_address_level(l_address);
	mmap_t* const	   mmap = get_virtual_map(address_level);
	chunk_info_t const chunk_info = get_chunk_info(mmap, l_address);

	mmap_info_t mmap_info;
	mmap_info.user = get_address_level(l_address);
	mmap_info.rw = get_chunk_rw(chunk_info.status);
	mmap_info.valid = get_mmap_info_valid(chunk_info.status, fault_rw, fault_level, mmap_info.user);

	return (mmap_info);
}

static inline bool get_chunk_rw(uint32_t const status) {
	return (status == MMAP_USED_RONLY) ? READ_ONLY : READ_WRITE;
}

static inline bool get_mmap_info_valid(uint32_t const status,
									   bool const	  fault_rw,
									   bool const	  fault_level,
									   bool const	  mmap_level) {
	bool valid = false;

	if (((status == MMAP_USED) || (status == MMAP_USED_RONLY && fault_rw == READ_ONLY)) &&
		(mmap_level >= fault_level)) {
		valid = true;
	}
	return (valid);
}
