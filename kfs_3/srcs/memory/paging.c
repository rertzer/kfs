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
	bool	  ok = true;
	uint32_t* address = k_mmap(PAGE_SIZE);
	if (address == NULL) {
		printk("panic!\n");
		ok = false;

	} else {
		ok = add_page_to_dir_page(offset, address,
								  PAGE_TABLE_SUPERVISOR | PAGE_TABLE_WRITE | PAGE_TABLE_PRESENT);
		if (ok == true) {
			uint8_t* memset_address = (uint8_t*)PAGE_DIR_BASE + (offset << 12);
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

void page_testing() {
	uint32_t offset = 0;
	char*	 good_string = "hello word\n";
	char*	 addr = v_mmap(1, SUPERVISOR_LEVEL, READ_WRITE);
	// char* fake_addr = (char*)0xD09DC300;
	// char* addr = (char*)0xc0400000;
	printk("%s\n", good_string);
	printk("my funny addr %08x has size %u \n", addr, v_size(addr));
	press_any();
	// virtual_memory_infos(NULL, 0);
	// godot();
	char cacahuete = addr[offset];
	printk("Got the cacahuete '%c'\n", cacahuete);
	addr[offset] = 'Z';
	printk("cacahuete (expect 'Z') %c\n", addr[offset]);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
bool add_page_entry(uint32_t l_address, uint32_t p_address, uint32_t flags) {}

uint32_t get_page_table_flags(mmap_info_t mmap_info) {
	uint32_t flags = 0;

	flags |= (mmap_info.valid == true) ? PAGE_TABLE_PRESENT : PAGE_TABLE_ABSENT;
	flags |= (mmap_info.user == true) ? PAGE_TABLE_USER : PAGE_TABLE_SUPERVISOR;
	flags |= (mmap_info.rw == true) ? PAGE_TABLE_WRITE : PAGE_TABLE_READ;

	return (flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////
static bool page_missing(uint32_t l_address, bool fault_level, bool fault_rw) {
	bool ok = true;

	mmap_info_t mmap_info = v_mmap_check((void*)l_address, fault_level, fault_rw);
	ok = mmap_info.valid;
	if (ok == true) {
		ok = dir_page_missing(l_address);
		if (ok == true) {
			uint32_t flags = get_page_table_flags(mmap_info);
			ok = add_page_table_entry(l_address, flags);
		}
	}

	return (ok);
}

static bool dir_page_missing(uint32_t l_address) {
	bool	 ok = true;
	uint32_t page_offset = get_dir_page_offset(l_address);

	uint32_t* addr = (uint32_t*)PAGE_DIR_ADDR + page_offset;
	// printk("page offset and value %u %08x\n", page_offset, *addr);
	if (*addr & PAGE_FAULT_P) {
		// printk("dir page entry present\n");
	} else {
		// printk("dir page entry missing\n");
		ok = create_page_table(page_offset);
		flush_tlb();
	}

	return (ok);
}

static bool add_page_table_entry(uint32_t l_address, uint32_t flags) {
	printk("page table missing \n");

	bool	  ok = true;
	uint32_t* page_address = get_page_table_address(l_address);
	uint32_t  page_offset = get_page_table_offset(l_address);
	// printk("page address is %08x\n", page_address);
	// printk("page offset is %08x\n", page_offset);
	uint32_t* entry = page_address + page_offset;

	void* address = k_mmap(PAGE_SIZE);
	if (address == NULL) {
		printk("page fault: error: no physical memory available\n");
		ok = false;
	} else {
		// printk("page table missing: virtual address is %08x \n", l_address);
		// printk("page table missing: physical address is %08x \n", address);
		*entry = (uint32_t)address | flags;

		flush_tlb();
		// printk("page table missing: value is %08x \n", *entry);
	}

	return (ok);
}
