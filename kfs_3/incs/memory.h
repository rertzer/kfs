#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void  init_memory();
void* p_map(uint32_t size);
#endif
