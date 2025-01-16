#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void	 init_memory();
void	 freeze_memory(uint8_t* addr, uint32_t len);
void*	 p_map(uint32_t size);
uint32_t get_page_index(void* addr);
#endif
