#include "kernel.h"
#include "memory.h"
#include "paging.h"

void* mbook(uint32_t size, bool level, bool rw) {
	// map virtual memory
	void* v_addr = v_mmap(size, level, rw);
	if (v_addr == NULL) {
		printk("mbook: virtual memory mapping failed\n");
		return (NULL);
	}
	size = v_size(v_addr);
	if (size / PAGE_SIZE != 0) {
		printk("mbook: page size error\n");
		printk("time to PANIC!!!!\n");
	}
	// map physical memory
	void* p_addr = k_mmap(size);
	if (p_addr == NULL) {
		printk("mbook: physical memory mapping failed\n");
		v_free(v_addr);
		return (NULL);
	}
	// write page_table
	mmap_info_t mmap_info = {level, rw, 1};
	uint32_t	flags = get_page_table_flags(mmap_info);
	uint32_t	page_v_addr = (uint32_t)v_addr;
	uint32_t	page_p_addr = (uint32_t)p_addr;
	while (size != 0) {
		add_page_entry(page_v_addr, page_p_addr, flags);
		page_v_addr += PAGE_SIZE;
		page_p_addr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	return (v_addr);
}
