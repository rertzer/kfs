#include "builtin.h"
#include "keycode.h"
#include "memory.h"

static void print_physical_memory_infos();
static void print_header_mem_infos(int* column_size);
static void print_row_mem_infos(int* column_size, uint32_t index, mem_info_t* mem_infos);
static void print_footer_mem_infos(int* column_size, mem_info_t mem_total);
static void print_virtual_memory_infos(bool level);

uint8_t memory_infos(size_t argc, char** argv) {
	(void)argc;
	(void)argv;

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

void print_mem_infos(mem_info_t* mem_infos) {
	mem_info_t mem_total = {0, 0, 0};
	int		   column_size[4] = {0};

	print_header_mem_infos(column_size);
	for (uint32_t i = 0; i <= MMAP_MAX_SHIFT; ++i) {
		print_row_mem_infos(column_size, i, mem_infos);
		mem_total.used += mem_infos[i].used;
		mem_total.free += mem_infos[i].free;
		mem_total.total += mem_infos[i].total;
	}
	print_footer_mem_infos(column_size, mem_total);
}

static void print_row_mem_infos(int* column_size, uint32_t index, mem_info_t* mem_infos) {
	printk("%c %-*u %c %-*d %c %-*d %c %-*d %c\n", VERTICAL_LINE, column_size[0] - 3, 1 << index, VERTICAL_LINE,
		   column_size[1] - 3, mem_infos[index].free, VERTICAL_LINE, column_size[2] - 3, mem_infos[index].used,
		   VERTICAL_LINE, column_size[3] - 3, mem_infos[index].total, VERTICAL_LINE);
}

static void print_footer_mem_infos(int* column_size, mem_info_t mem_total) {
	printk("%c %*s %c %-*d %c %-*d %c %-*d %c\n", VERTICAL_LINE, column_size[0] - 3, "total pages", VERTICAL_LINE,
		   column_size[1] - 3, mem_total.free, VERTICAL_LINE, column_size[2] - 3, mem_total.used, VERTICAL_LINE,
		   column_size[3] - 3, mem_total.total, VERTICAL_LINE);
}

static void print_header_mem_infos(int* column_size) {
	printk("%c %-6s %n%c %-6s %n%c %-6s %n%c %-6s %n%c\n", VERTICAL_LINE, "pages (in chunks of size n)",
		   column_size + 0, VERTICAL_LINE, "free", column_size + 1, VERTICAL_LINE, "used", column_size + 2,
		   VERTICAL_LINE, "total", column_size + 3, VERTICAL_LINE);
	column_size[3] -= column_size[2];
	column_size[2] -= column_size[1];
	column_size[1] -= column_size[0];
}

static void print_virtual_memory_infos(bool level) {
	mem_info_t mem_infos[MMAP_MAX_SHIFT + 1];
	get_virtual_memory_infos((mem_info_t*)&mem_infos, level);

	char* level_name = (level == USER_LEVEL) ? "User" : "Kernel";
	printk("%s virtual memory infos\n", level_name);
	print_mem_infos(mem_infos);
}
