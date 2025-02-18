#include "builtin.h"
#include "keycode.h"
#include "memory.h"
#include "paging.h"

static uint32_t		   mbook_get_flags(bool const level, bool const rw);
static void			   mbook_write_page_table(void const* const v_addr,
											  void const* const p_addr,
											  uint32_t const	size,
											  uint32_t const	flags);
static inline uint32_t get_v_page_nb(void const* const v_addr);

void* mbook(uint32_t size, bool const level, bool const rw) {
	void* const v_addr = v_mmap(size, level, rw);
	if (v_addr == NULL) {
		printk("mbook: virtual memory mapping failed\n");
		return (NULL);
	}
	size = v_size(v_addr);

	void const* const p_addr = k_mmap(size);
	if (p_addr == NULL) {
		printk("mbook: physical memory mapping failed\n");
		v_free(v_addr);
		return (NULL);
	}

	uint32_t const flags = mbook_get_flags(level, rw);
	mbook_write_page_table(v_addr, p_addr, size, flags);

	return (v_addr);
}

static uint32_t mbook_get_flags(bool const level, bool const rw) {
	return (get_page_table_flags((mmap_info_t const){level, rw, 1}));
}

static void mbook_write_page_table(void const* const v_addr,
								   void const* const p_addr,
								   uint32_t const	 size,
								   uint32_t const	 flags) {
	uint32_t	   page_v_addr = (uint32_t)v_addr;
	uint32_t	   page_p_addr = (uint32_t)p_addr;
	uint32_t const page_nb = size / PAGE_SIZE;

	for (uint32_t i = 0; i < page_nb; ++i) {
		add_page_entry(page_v_addr, page_p_addr, flags);
		page_v_addr += PAGE_SIZE;
		page_p_addr += PAGE_SIZE;
	}
}

void munbook(void const* const v_addr) {
	v_free(v_addr);
	k_free(get_physical_address((uint32_t const)v_addr));
	free_page_table(v_addr, get_v_page_nb(v_addr));
}

static inline uint32_t get_v_page_nb(void const* const v_addr) {
	return (v_size(v_addr) / PAGE_SIZE);
}

//////////////////////////////////////////////////////////////////////////
void mbook_test() {
	uint8_t* addr = mbook(14 * PAGE_SIZE, SUPERVISOR_LEVEL, READ_WRITE);
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
	munbook(addr);
	printk("memory freeeeed\n");
	addr = mbook(17 * PAGE_SIZE, SUPERVISOR_LEVEL, READ_WRITE);
	if (addr == NULL) {
		printk("address is NULL. Lets see what's going on...\n");
		press_any();
		memory_infos(NULL, 0);
		press_any();
	}
	addr[007] = 'Z';
	printk("at index 0: %c ('Z' expected)\n", addr[007]);
	far_offset = 4096 * 12 + 666;
	addr[far_offset] = 'A';
	printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);

	// printk("at index 0: %c ('Z' expected)\n", addr[666]);
	// printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
	// far_offset = 4096 * 17 + 9999;
	// printk("writting in a too far offset\n");
	// addr[far_offset] = 'A';
	// printk("at index %u: %c ('A expected)\n", far_offset, addr[far_offset]);
}
