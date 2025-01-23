#include "kernel.h"
#include "memory.h"
#include "mmap.h"

extern uint32_t v_mmap_start;
mmap_t			virt_mmap;

void init_v_memory() {
	init_mmap(&virt_mmap, (uint8_t*)&v_mmap_start, 0, VIRTUAL_MEMORY_KIB_SIZE);

	set_all_memory_free(&virt_mmap);
}
