#include "memory.h"
#include "boot_infos.h"
#include "kernel.h"
#include "mmap.h"

extern uint32_t p_mmap_start;

mmap_t p_mmap;

void init_memory() {
	init_mmap(&p_mmap, (uint8_t*)&p_mmap_start);
	set_memory_size(&p_mmap, boot_infos_get_mem_size());
	boot_infos_memory_map_freeze(&p_mmap);
}
