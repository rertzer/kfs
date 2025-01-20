#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mmap.h"
#define PAGE_SIZE 4096
#define KERNEL_SIZE 1024 * PAGE_SIZE

void	   init_memory();
mem_info_t get_memory_infos();
void*	   p_map(uint32_t size);
#endif
