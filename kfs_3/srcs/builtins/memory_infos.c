#include "builtin.h"
#include "memory.h"

uint8_t memory_infos(char* pointer, size_t len) {
	mem_info_t mem_infos = get_memory_infos();
	printk("page frames:\n\tfree:  %u\n\tused:  %u\n\ttotal: %u\n", mem_infos.free, mem_infos.used,
		   mem_infos.total);
	return (0);
}
