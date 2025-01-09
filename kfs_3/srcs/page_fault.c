#include "interrupts.h"
#include "kernel.h"
#include "paging.h"

static bool page_missing(uint32_t l_address);
static bool dir_page_missing(uint32_t l_address);
static bool page_table_missing(uint32_t l_address);

void page_fault_handler(uint32_t l_address, uint32_t error_code) {
	bool ok = true;
	printk("page fault!\nerror code is 0x%08x\n", error_code);
	uint32_t access_error =
		error_code & (PAGE_FAULT_P | PAGE_FAULT_W | PAGE_FAULT_PKR | PAGE_FAULT_SS);
	printk("access_error: 0x%08x\n", access_error);
	if (access_error) {
		printk("panic\n");
		ok = false;
	} else {
		printk("don't panic\nlinear address is 0x%08x\n", l_address);
		ok = page_missing(l_address);
	}
	if (ok == false) {
		printk("Waiting for Godot\n");
		godot();
	}
	flush_tlb();
	printk("page handling done\n");
	tabledump();
	godot();
	;
}

static bool page_missing(uint32_t l_address) {
	bool ok = true;

	ok = dir_page_missing(l_address);
	if (ok == true) {
		ok = page_table_missing(l_address);
	}
	return (ok);
}

static bool dir_page_missing(uint32_t l_address) {
	bool	 ok = true;
	uint32_t page_offset = get_dir_page_offset(l_address);
	// page_offset = get_dir_page_offset(0xc0102c00);	// !!!!!!!!!!!!!!!!!!!!!

	uint32_t* addr = (uint32_t*)PAGE_DIR_ADDR + page_offset;
	printk("page offset and value %u %08x\n", page_offset, *addr);
	if (*addr & PAGE_FAULT_P) {
		printk("dir page entry present\n");
	} else {
		printk("dir page entry missing\n");
		ok = create_page_table(page_offset);
	}

	return (ok);
}

static bool page_table_missing(uint32_t l_address) {
	bool ok = true;
	return (ok);
}
