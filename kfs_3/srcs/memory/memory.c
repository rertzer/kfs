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
void get_memory_infos(mem_info_t* mem_infos) {
	get_mmap_infos(&p_mmap, mem_infos);
}

void* k_mmap(uint32_t size) {
	size /= PAGE_SIZE;
	size = round_up_power_two(size);
	if (size > 15) {
		printk("k_mmap error: max size allowed is 2^15\n");
		return (NULL);
	}

	chunk_t chunk = get_free_chunk(&p_mmap, size);
	if (chunk.status != MMAP_FREE) {
		return (NULL);
	}
	chunk.status = MMAP_USED;
	set_chunk_status(&p_mmap, chunk);
	void* address = get_chunk_address(chunk);
	return (address);
}

uint32_t k_size(void* addr) {
	return (get_size_by_address(&p_mmap, addr));
}

uint8_t k_free(void* addr) {
	return (free_by_address(&p_mmap, addr));
}
