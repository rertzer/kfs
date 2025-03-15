#include "paging.h"
#include "kernel.h"
#include "keycode.h"
#include "memory.h"
#include "panic.h"

#include "utils.h" //for getting ft_memset

extern uint32_t const page_dir;

static void				free_page(void const* const l_address);
static inline uint32_t	get_dir_page_offset(uint32_t const l_address);
static inline uint32_t* get_page_table_address(uint32_t const l_address);
static inline uint32_t	get_page_table_offset(uint32_t const l_address);
static void				clear_page_table(uint32_t const offset);
static inline uint32_t* get_dir_page_entry(uint32_t const offset);
static void				add_page_to_dir_page(uint32_t const	 page_offset,
											 uint32_t* const page_physical_address,
											 uint32_t const	 flags);
static inline bool		page_present(uint32_t const* const addr);

static inline uint32_t get_dir_page_offset(uint32_t const l_address) {
	return ((l_address & (0x3FF << DIR_PAGE_SHIFT)) >> DIR_PAGE_SHIFT);
}

static inline uint32_t* get_page_table_address(uint32_t const l_address) {
	return ((uint32_t*)(PAGE_DIR_BASE + (get_dir_page_offset(l_address) << PAGE_TABLE_SHIFT)));
}

static inline uint32_t get_page_table_offset(uint32_t const l_address) {
	return ((l_address & (0x3FF << PAGE_TABLE_SHIFT)) >> PAGE_TABLE_SHIFT);
}

static inline uint32_t* get_dir_page_entry(uint32_t const offset) {
	return ((uint32_t*)PAGE_DIR_ADDR + offset);
}

void create_page_table(uint32_t const offset) {
	uint32_t* const address = k_mmap(PAGE_SIZE);
	if (address == NULL) {
		panic("unable to create page table: no physical memory available");
	}
	add_page_to_dir_page(offset, address, PAGE_TABLE_SUPERVISOR | PAGE_TABLE_WRITE | PAGE_TABLE_PRESENT);
	clear_page_table(offset);
}

static void clear_page_table(uint32_t const offset) {
	uint8_t* const memset_address = (uint8_t*)PAGE_DIR_BASE + (offset << PAGE_TABLE_SHIFT);
	ft_memset(memset_address, 0, PAGE_SIZE);
}

static void add_page_to_dir_page(uint32_t const	 page_offset,
								 uint32_t* const page_physical_address,
								 uint32_t const	 flags) {
	uint32_t* const entry = get_dir_page_entry(page_offset);
	*entry = (uint32_t)page_physical_address | flags;
}

uint32_t get_page_table_flags(mmap_info_t const mmap_info) {
	uint32_t flags = 0;

	flags |= (mmap_info.valid == true) ? PAGE_TABLE_PRESENT : PAGE_TABLE_ABSENT;
	flags |= (mmap_info.user == true) ? PAGE_TABLE_USER : PAGE_TABLE_SUPERVISOR;
	flags |= (mmap_info.rw == true) ? PAGE_TABLE_WRITE : PAGE_TABLE_READ;

	return (flags);
}

void add_page_entry(uint32_t const l_address, uint32_t const p_address, uint32_t const flags) {
	confirm_dir_page(l_address);
	set_page_table_entry(l_address, p_address, flags);
}
bool page_table_exist(void const* const l_address) {
	uint32_t const		  page_offset = get_dir_page_offset((uint32_t)l_address);
	uint32_t const* const addr = get_dir_page_entry(page_offset);
	return (page_present(addr));
}

void confirm_dir_page(uint32_t const l_address) {
	uint32_t const page_offset = get_dir_page_offset(l_address);

	uint32_t const* const addr = get_dir_page_entry(page_offset);
	if (page_present(addr) == false) {
		create_page_table(page_offset);
	}
}

static inline bool page_present(uint32_t const* const addr) {
	return (*addr & PAGE_FAULT_P);
}

void set_page_table_entry(uint32_t const l_address, uint32_t const p_address, uint32_t const flags) {
	uint32_t* const entry = get_table_entry_by_l_address(l_address);
	*entry = p_address | flags;
}

uint32_t* get_table_entry_by_l_address(uint32_t const l_address) {
	return (get_page_table_address(l_address) + get_page_table_offset(l_address));
}

void* get_physical_address(uint32_t const l_address) {
	return ((void*)*get_table_entry_by_l_address(l_address));
}

void free_page_table(void const* l_address, uint32_t const page_nb) {
	for (uint32_t offset = 0; offset < page_nb; ++offset) {
		free_page(l_address);
		l_address += PAGE_SIZE;
	}
	flush_tlb();
}

static void free_page(void const* const l_address) {
	uint32_t* const entry = get_table_entry_by_l_address((uint32_t)l_address);
	*entry = 0;
}

//////////////////////////////////////////////////////////////////////////////////
void page_testing() {
	uint32_t offset = 0;
	char*	 good_string = "hello word\n";
	char*	 addr = v_mmap(PAGE_SIZE, SUPERVISOR_LEVEL, READ_WRITE);
	// char* addr = v_mmap(1, SUPERVISOR_LEVEL, READ_ONLY);
	printk("%s\n", good_string);
	printk("my funny addr %08x has size %u \n", addr, v_size(addr));
	// printk("\ntesting access rights, panic expected\n");
	press_any();
	char cacahuete = addr[offset];
	printk("Got the cacahuete '%c'\n", cacahuete);
	addr[offset] = 'Z';
	printk("cacahuete (expect 'Z') %c\n", addr[offset]);
	// printk("\ntesting fake address, panic expected\n");
	// press_any();
	// char* fake_addr = (char*)0xD09DC300;
	// printk("cacahuete is %c\n", *fake_addr);
	// printk("If you see this, the panic test failed :(\n)");
}
