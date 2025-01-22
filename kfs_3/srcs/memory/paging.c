#include "paging.h"
#include "kernel.h"
#include "memory.h"

extern uint32_t page_dir;

uint32_t get_dir_page_offset(uint32_t l_address) {
	printk("get_dir_page_offset ");
	uint32_t offset = l_address & (0x3FF << 22);
	printk("%08x ", offset);
	offset >>= 22;

	printk("%08x \n", offset);
	return (offset);
}

uint32_t* get_page_table_address(uint32_t l_address) {
	uint32_t page_offset = get_dir_page_offset(l_address);
	page_offset <<= 12;
	uint32_t addr = PAGE_DIR_BASE + page_offset;
	printk("%08x + %08x = %08x\n", PAGE_DIR_BASE, page_offset, addr);
	return ((uint32_t*)addr);
}

uint32_t get_page_table_offset(uint32_t l_address) {
	printk("get page table offset ");
	uint32_t offset = l_address & (0x3FF << 12);
	printk("%08x ", offset);
	offset >>= 12;
	printk("%08x \n", offset);
	return (offset);
}

bool create_page_table(uint32_t offset) {
	// static uint32_t count;

	bool ok = true;
	// frame_alloc ;
	// uint32_t address = 0x009EC000 + PAGE_SIZE * count;	// temporary fake address
	// ++count;
	uint32_t* address = k_mmap(PAGE_SIZE);
	if (address == NULL) {
		printk("panic!\n");
		ok = false;

	} else {
		ok = add_page_to_dir_page(offset, address,
								  PAGE_TABLE_SUPERVISOR | PAGE_TABLE_WRITE | PAGE_TABLE_PRESENT);
		if (ok == true) {
			flush_tlb();
			uint32_t offoffset = (offset) << 12;
			uint8_t* memset_address = (uint8_t*)PAGE_DIR_BASE + offoffset;
			ft_memset(memset_address, 0, PAGE_SIZE);
			flush_tlb();
		}
	}

	return (ok);
}

bool add_page_to_dir_page(uint32_t page_offset, uint32_t* page_physical_address, uint32_t flags) {
	bool ok = true;

	uint32_t* entry = (uint32_t*)PAGE_DIR_ADDR + page_offset;
	printk("offset is %u, entry address is: %08x\n", page_offset, entry);
	*entry = (uint32_t)page_physical_address | flags;

	printk("new entry value is %08x\n", *entry);

	return (ok);
}
