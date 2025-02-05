#include "interrupts.h"
#include "kernel.h"
#include "memory.h"
#include "paging.h"

static bool page_missing(uint32_t l_address, bool fault_level, bool fault_rw);
static bool add_page_table_entry(uint32_t l_address, uint32_t flags);

void page_fault_handler(uint32_t l_address, uint32_t error_code) {
	bool ok = true;
	printk("page fault!\nerror code is 0x%08x\n", error_code);
	uint32_t access_error = error_code & (PAGE_FAULT_P | PAGE_FAULT_W);
	printk("access_error: 0x%08x\n", access_error);
	if (access_error) {
		printk("Panic\nlinear address is 0x%08x\n", l_address);
		ok = false;
	} else {
		printk("don't panic\nlinear address is 0x%08x\n", l_address);
		ok = page_missing(l_address, (error_code & PAGE_FAULT_W), (error_code & PAGE_FAULT_USER));
	}
	if (ok == false) {
		printk("page fault! invalid address 0x%08x\n", l_address);
		godot();
	}
	flush_tlb();
	printk("page handling done\n");
}

static bool page_missing(uint32_t l_address, bool fault_level, bool fault_rw) {
	bool ok = true;

	mmap_info_t mmap_info = v_mmap_check((void*)l_address, fault_level, fault_rw);
	ok = mmap_info.valid;
	if (ok == true) {
		ok = confirm_dir_page(l_address);
		if (ok == true) {
			uint32_t flags = get_page_table_flags(mmap_info);
			ok = add_page_table_entry(l_address, flags);
		}
	}

	return (ok);
}

static bool add_page_table_entry(uint32_t l_address, uint32_t flags) {
	printk("page table missing \n");

	bool  ok = true;
	void* p_address = k_mmap(PAGE_SIZE);
	if (p_address == NULL) {
		printk("page fault: error: no physical memory available\n");
		ok = false;
	} else {
		ok = set_page_table_entry(l_address, (uint32_t)p_address, flags);
	}

	return (ok);
}
