#include "paging.h"
#include "kernel.h"

extern uint32_t page_dir;

uint32_t* get_dir_page() {
	uint32_t* higher_page_dir = to_upper_kernel(&page_dir);
	return (higher_page_dir);
}

uint32_t get_dir_page_offset(uint32_t l_address) {
	uint32_t offset = l_address & (0x3FF << 22);
	offset >>= 22;

	return (offset);
}

uint32_t create_page_table(uint32_t offset) {
	// v_map() one page;
	// frame_alloc v_mapped page;
	return (physical page address);
}
