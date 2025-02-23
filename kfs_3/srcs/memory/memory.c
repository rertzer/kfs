#include "memory.h"
#include "boot_infos.h"
#include "builtin.h"
#include "kernel.h"
#include "keycode.h"
#include "mmap.h"

extern uint32_t const p_mmap_start;
extern uint32_t const stack_bottom;
extern uint32_t const stack_top;

mmap_t p_mmap;

void init_memory() {
	uint32_t memory_size = boot_infos_get_mem_size();
	printk("- physical memory size is %u Kb\t", memory_size);
	init_mmap(&p_mmap, (uint8_t*)&p_mmap_start, 0, memory_size);
	boot_infos_memory_map_freeze(&p_mmap);
	book_memory(&p_mmap, 0, KERNEL_SIZE, MMAP_USED);
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
	void* address = get_chunk_address(&p_mmap, chunk);
	return (address);
}

uint32_t k_size(void* addr) {
	return (get_size_by_address(&p_mmap, addr));
}

uint8_t k_free(void* addr) {
	if (addr == 0) {
		return (0);
	}
	return (free_by_address(&p_mmap, addr));
}

//////////////////////////////////////////////////////////////////////////////////
void memory_test() {
	size_t block_nb = 250;
	size_t block_size = 7;
	void*  addrs[1000];

	for (size_t i = 0; i < block_nb; ++i) {
		addrs[i] = k_mmap(PAGE_SIZE << block_size);
		printk("k_mmap addrs is %08x\n", addrs[i]);
		if (addrs[i] == 0) {
			press_any();
		}
	}
	press_any();
	memory_infos(NULL, 0);
	press_any();

	for (size_t i = 0; i < block_nb; ++i) {
		if (addrs[i] != 0) {
			k_free(addrs[i]);
		}
		printk("free addrs is %08x\n", addrs[i]);
	}

	press_any();
	memory_infos(NULL, 0);
	press_any();
}
