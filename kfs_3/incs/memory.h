#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define KERNEL_SIZE 1024 * PAGE_SIZE
void  init_memory();
void* p_map(uint32_t size);
#endif
