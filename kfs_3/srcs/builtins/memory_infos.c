#include "builtin.h"
#include "keycode.h"
#include "memory.h"

static void print_physical_memory_infos();
static void print_virtual_memory_infos(bool user);
static void print_mem_infos(mem_info_t* mem_infos);

uint8_t memory_infos(char* pointer, size_t len) {
	(void)pointer;
	(void)len;

	print_physical_memory_infos();
	press_any();
	print_virtual_memory_infos(USER_LEVEL);
	press_any();
	print_virtual_memory_infos(SUPERVISOR_LEVEL);
	return (0);
}

static void print_physical_memory_infos() {
	mem_info_t mem_infos[MMAP_MAX_SHIFT + 1];

	printk("Physical memory infos\n");
	get_memory_infos((mem_info_t*)&mem_infos);
	print_mem_infos((mem_info_t*)mem_infos);
}

static void print_mem_infos(mem_info_t* mem_infos) {
	mem_info_t mem_total = {0, 0, 0};

	for (uint32_t i = 0; i <= MMAP_MAX_SHIFT; ++i) {
		printk("pages (in chunks of size %u):\tfree:  %u\tused:  %u\ttotal: %u\n", 1 << i, mem_infos[i].free,
			   mem_infos[i].used, mem_infos[i].total);
		mem_total.used += mem_infos[i].used;
		mem_total.free += mem_infos[i].free;
		mem_total.total += mem_infos[i].total;
	}
	printk("total pages:\tfree:  %u\tused:  %u\ttotal: %u\n", mem_total.free, mem_total.used, mem_total.total);
}

static void print_virtual_memory_infos(bool level) {
	mem_info_t mem_infos[MMAP_MAX_SHIFT + 1];
	get_virtual_memory_infos((mem_info_t*)&mem_infos, level);

	char* level_name = (level == USER_LEVEL) ? "User" : "Kernel";
	printk("%s virtual memory infos\n", level_name);
	print_mem_infos(mem_infos);
}
