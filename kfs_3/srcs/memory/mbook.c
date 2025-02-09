#include "builtin.h"
#include "kernel.h"
#include "memory.h"
#include "paging.h"

static uint32_t mbook_get_flags(bool level, bool rw);
static uint8_t	mbook_write_page_table(void* v_addr, void* p_addr, uint32_t size, uint32_t flags);

void* mbook(uint32_t size, bool level, bool rw) {
	// map virtual memory
	void* v_addr = v_mmap(size, level, rw);
	if (v_addr == NULL) {
		printk("mbook: virtual memory mapping failed\n");
		return (NULL);
	}
	size = v_size(v_addr);

	void* p_addr = k_mmap(size);
	if (p_addr == NULL) {
		printk("mbook: physical memory mapping failed\n");
		v_free(v_addr);
		return (NULL);
	}
	uint32_t flags = mbook_get_flags(level, rw);
	if (mbook_write_page_table(v_addr, p_addr, size, flags) != 0) {
		printk("mbook: page table error\n");
		k_free(p_addr);
		v_free(v_addr);
		return (NULL);
	}

	return (v_addr);
}

static uint32_t mbook_get_flags(bool level, bool rw) {
	mmap_info_t mmap_info = {level, rw, 1};
	return (get_page_table_flags(mmap_info));
}

static uint8_t mbook_write_page_table(void* v_addr, void* p_addr, uint32_t size, uint32_t flags) {
	uint8_t	 error = 0;
	uint32_t page_v_addr = (uint32_t)v_addr;
	uint32_t page_p_addr = (uint32_t)p_addr;
	uint32_t page_nb = size / PAGE_SIZE;

	for (uint32_t i = 0; i < page_nb; ++i) {
		bool ok = add_page_entry(page_v_addr, page_p_addr, flags);
		if (ok == false) {
			free_page_table(v_addr, i);
			error = 1;
			break;
		}
		page_v_addr += PAGE_SIZE;
		page_p_addr += PAGE_SIZE;
	}
	return (error);
}

void munbook(void* v_addr) {
	uint32_t page_nb = v_size(v_addr) / PAGE_SIZE;
	v_free(v_addr);
	void* p_addr = get_physical_address((uint32_t)v_addr);
	k_free(p_addr);
	free_page_table(v_addr, page_nb);
}

void mbook_test() {
	// memory_infos(NULL, 0);
	// press_any();
	printk("mbook\n");
	uint8_t* addr = mbook(14 * PAGE_SIZE, SUPERVISOR_LEVEL, READ_WRITE);
	printk("addr is %08x\n", addr);
	if (addr == NULL) {
		printk("address is NULL. Lets see what's going on...\n");
		press_any();
		memory_infos(NULL, 0);
		press_any();
	}
	addr[666] = 'Z';
	printk("at index 0: %c ('Z' expected)\n", addr[666]);
	uint32_t far_offset = 4096 * 14 + 666;
	addr[far_offset] = 'A';
	printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
	// memory_infos(NULL, 0);
	// press_any();
	// munbook(addr);
	// printk("memory freeeeed\n");
	// printk("at index 0: %c ('Z' expected)\n", addr[666]);
	// printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
	// far_offset = 4096 * 17 + 9999;
	// printk("writting in a too far offset\n");
	// addr[far_offset] = 'A';
	// printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
}
