#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mmap.h"
#define PAGE_SIZE 4096
#define KERNEL_SIZE 1024 * PAGE_SIZE

void	 init_memory();
void	 get_memory_infos(mem_info_t* mem_infos);
void*	 k_mmap(uint32_t size);
uint32_t k_size(void* addr);
uint8_t	 k_free(void* addr);
#endif
