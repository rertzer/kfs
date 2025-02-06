#ifndef PAGING_H
#define PAGING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "memory.h"

#define PAGE_FAULT_P 1
#define PAGE_FAULT_W 2
#define PAGE_FAULT_USER 4
#define PAGE_TABLE_USER 0
#define PAGE_TABLE_SUPERVISOR 4
#define PAGE_TABLE_READ 0
#define PAGE_TABLE_WRITE 2
#define PAGE_TABLE_ABSENT 0
#define PAGE_TABLE_PRESENT 1

#define PAGE_DIR_ADDR 0xFFFFF000
#define PAGE_DIR_BASE 0xFFC00000

uint32_t  get_dir_page_offset(uint32_t l_address);
uint32_t* get_page_table_address(uint32_t l_address);
uint32_t  get_page_table_offset(uint32_t l_address);
bool add_page_to_dir_page(uint32_t page_offset, uint32_t* page_physical_address, uint32_t flags);
bool create_page_table(uint32_t offset);
bool add_page_entry(uint32_t l_address, uint32_t p_address, uint32_t flags);
uint32_t get_page_table_flags(mmap_info_t mmap_info);
bool	 confirm_dir_page(uint32_t l_address);
bool	 set_page_table_entry(uint32_t l_address, uint32_t p_address, uint32_t flags);

#endif
