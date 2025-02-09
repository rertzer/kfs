#include "builtin.h"
#include "kernel.h"
#include "memory.h"

static void virtual_memory_infos_by_level(bool user);
static void print_mem_infos(mem_info_t* mem_infos);

uint8_t memory_infos(char* pointer, size_t len) {
	mem_info_t mem_infos[MMAP_MAX_SHIFT + 1];

	printk("physical memory\n");
	get_memory_infos((mem_info_t*)&mem_infos);
	print_mem_infos((mem_info_t*)mem_infos);
	press_any();
	virtual_memory_infos(NULL, 0);

	return (0);
}

static void print_mem_infos(mem_info_t* mem_infos) {
	mem_info_t mem_total = {0, 0, 0};

	for (uint32_t i = 0; i <= MMAP_MAX_SHIFT; ++i) {
		printk("pages (in chunks of size %u):\tfree:  %u\tused:  %u\ttotal: %u\n", 1 << i,
			   mem_infos[i].free, mem_infos[i].used, mem_infos[i].total);
		mem_total.used += mem_infos[i].used;
		mem_total.free += mem_infos[i].free;
		mem_total.total += mem_infos[i].total;
	}
	printk("total pages:\tfree:  %u\tused:  %u\ttotal: %u\n", mem_total.free, mem_total.used,
		   mem_total.total);
}

uint8_t virtual_memory_infos(char* pointer, size_t len) {
	printk("user virtual memory infos\n");
	virtual_memory_infos_by_level(USER_LEVEL);
	press_any();
	printk("kernel virtual memory infos\n");
	virtual_memory_infos_by_level(SUPERVISOR_LEVEL);
	return (0);
}

static void virtual_memory_infos_by_level(bool level) {
	mem_info_t mem_infos[MMAP_MAX_SHIFT + 1];

	get_virtual_memory_infos((mem_info_t*)&mem_infos, level);
	print_mem_infos(mem_infos);
}
