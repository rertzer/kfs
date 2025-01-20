#include "memory.h"
#include "boot_infos.h"
#include "kernel.h"
#include "mmap.h"

extern uint32_t p_mmap_start;

mmap_t p_mmap;

void init_memory() {
	init_mmap(&p_mmap, (uint8_t*)&p_mmap_start);
	uint32_t memory_size = 1024 * boot_infos_get_mem_size();
	set_memory_size(&p_mmap, memory_size);
	boot_infos_memory_map_freeze(&p_mmap);
	freeze_memory(&p_mmap, 0, KERNEL_SIZE);
}
mem_info_t get_memory_infos() {
	mem_info_t mem_infos;
	mem_infos = get_mmap_infos(&p_mmap);
	return (mem_infos);
}
