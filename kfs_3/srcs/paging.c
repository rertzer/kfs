#include "paging.h"
#include "kernel.h"

extern uint32_t page_dir;

uint32_t get_dir_page_offset(uint32_t l_address) {
	uint32_t offset = l_address & (0x3FF << 22);
	offset >>= 22;

	return (offset);
}

bool create_page_table(uint32_t offset) {
	static uint32_t count;

	bool ok = true;
	// frame_alloc ;
	uint32_t address = 0x009EC000 + PAGE_SIZE * count;	// temporary fake address
	++count;
	printk("create page at address %08x\n", address);
	if (address == 0) {
		printk("panic!\n");
		ok = false;

	} else {
		ok = add_page_to_dir_page(offset, (uint32_t*)address,
								  PAGE_TABLE_SUPERVISOR | PAGE_TABLE_WRITE | PAGE_TABLE_PRESENT);
		if (ok == true) {
			flush_tlb();
			ft_memset((char*)(PAGE_DIR_BASE + 4 * offset), 0, PAGE_SIZE);
		}
	}

	return (ok);  //(physical page address);
}

bool add_page_to_dir_page(uint32_t page_offset, uint32_t* page_physical_address, uint32_t flags) {
	bool ok = true;

	uint32_t* entry = (uint32_t*)PAGE_DIR_ADDR + page_offset;
	printk("entry address is: %08x\n", entry);
	*entry = (uint32_t)page_physical_address | flags;

	printk("new entry value is %08x\n", *entry);

	return (ok);
}
