#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 2 bits per page:
 *  00 not available
 *  01 freezed (multiboot infos, etc.)
 *  10 used block
 *  11 free block
 *
 * */

#define MMAP_UNAVAILABLE 0
#define MMAP_FREEZED 1
#define MMAP_USED 2
#define MMAP_FREE 3

#define PAGES_PER_BYTE 4
#define MMAP_PAGE_MASK 3

void	 init_memory();
void	 freeze_memory(uint8_t* addr, uint32_t len);
void*	 p_map(uint32_t size);
uint32_t get_page_index(void* addr);
#endif
