#ifndef PAGING_H
#define PAGING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_FAULT_P 1
#define PAGE_FAULT_W 2
#define PAGE_FAULT_PKR 32
#define PAGE_FAULT_SS 64
#define PAGE_TABLE_USER 0
#define PAGE_TABLE_SUPERVISOR 4
#define PAGE_TABLE_READ 0
#define PAGE_TABLE_WRITE 2
#define PAGE_TABLE_ABSENT 0
#define PAGE_TABLE_PRESENT 1
#define PAGE_SIZE 4096

#define PAGE_DIR_ADDR 0xFFFFF000
#define PAGE_DIR_BASE 0xFFC00000

uint32_t get_dir_page_offset(uint32_t l_address);
bool add_page_to_dir_page(uint32_t page_offset, uint32_t* page_physical_address, uint32_t flags);
bool create_page_table(uint32_t offset);
#endif
