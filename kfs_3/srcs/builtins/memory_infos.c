#include "builtin.h"
#include "memory.h"

uint8_t memory_infos(char* pointer, size_t len) {
	mem_info_t mem_infos[MMAP_MAX_SIZE + 1];
	mem_info_t mem_total = {0, 0, 0};

	get_memory_infos((mem_info_t*)&mem_infos);
	// godot();

	for (uint32_t i = 0; i <= MMAP_MAX_SIZE; ++i) {
		printk("page frames (chunks of size %u):\tfree:  %u\tused:  %u\ttotal: %u\n", 1 << i,
			   mem_infos[i].free, mem_infos[i].used, mem_infos[i].total);
		mem_total.used += mem_infos[i].used;
		mem_total.free += mem_infos[i].free;
		mem_total.total += mem_infos[i].total;
	}
	printk("total page frames:\tfree:  %u\tused:  %u\ttotal: %u\n", mem_total.free, mem_total.used,
		   mem_total.total);
	return (0);
}
