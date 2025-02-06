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
		bool ok = add_page_entry(page_v_addr, page_p_addr, flags);
		if (ok == false) {
			printk("mbook: page table error\n");
			k_free(p_addr);
			v_free(v_addr);
			// il faut aussi clean les tables
			return (NULL);
		}
		page_v_addr += PAGE_SIZE;
		page_p_addr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	return (v_addr);
}

void munbook(void* v_addr) {
	uint32_t page_nb = v_size(v_addr) / PAGE_SIZE;
	v_free(v_addr);
	void* p_addr = get_physical_address((uint32_t)v_addr);
	k_free(p_addr);
	free_page_table(v_addr, page_nb);
}

void mbook_test() {
	printk("mbook\n");
	uint8_t* addr = mbook(14 * PAGE_SIZE, SUPERVISOR_LEVEL, READ_WRITE);
	printk("addr is %08x\n", addr);
	addr[666] = 'Z';
	printk("at index 0: %c ('Z' expected)\n", addr[666]);
	uint32_t far_offset = 4096 * 14 + 666;
	addr[far_offset] = 'A';
	printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
	munbook(addr);
	printk("memory freeeeed\n");
	// printk("at index 0: %c ('Z' expected)\n", addr[666]);
	// printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
	// far_offset = 4096 * 17 + 9999;
	// printk("writting in a too far offset\n");
	// addr[far_offset] = 'A';
	// printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
}
