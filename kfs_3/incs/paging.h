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

#define DIR_PAGE_SHIFT 22
#define PAGE_TABLE_SHIFT 12

void	  create_page_table(uint32_t const offset);
void	  add_page_entry(uint32_t const l_address, uint32_t const p_address, uint32_t const flags);
uint32_t  get_page_table_flags(mmap_info_t const mmap_info);
void*	  get_physical_address(uint32_t const l_address);
void	  confirm_dir_page(uint32_t const l_address);
void	  set_page_table_entry(uint32_t const l_address, uint32_t const p_address, uint32_t const flags);
uint32_t* get_table_entry_by_l_address(uint32_t const l_address);
void	  free_page_table(void const* l_address, uint32_t const page_nb);

#endif
