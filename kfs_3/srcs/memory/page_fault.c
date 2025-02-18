#include "interrupts.h"
#include "memory.h"
#include "paging.h"
#include "panic.h"

static void page_missing(uint32_t l_address, uint32_t error_code);
static void add_page_table_entry(uint32_t l_address, uint32_t flags);

void page_fault_handler(uint32_t l_address, uint32_t error_code) {
	if (error_code & (PAGE_FAULT_P | PAGE_FAULT_W)) {
		panic("page fault access error");
	} else {
		page_missing(l_address, error_code);
	}
}

static void page_missing(uint32_t l_address, uint32_t error_code) {
	mmap_info_t mmap_info = v_mmap_check((void*)l_address, error_code & PAGE_FAULT_W, error_code & PAGE_FAULT_USER);
	if (mmap_info.valid == true) {
		confirm_dir_page(l_address);
		add_page_table_entry(l_address, get_page_table_flags(mmap_info));

	} else {
		panic("page fault: invalid address");
	}
}

static void add_page_table_entry(uint32_t l_address, uint32_t flags) {
	void* p_address = k_mmap(PAGE_SIZE);

	if (p_address == NULL) {
		panic("page fault: no physical memory available\n");
	} else {
		set_page_table_entry(l_address, (uint32_t)p_address, flags);
	}
}
