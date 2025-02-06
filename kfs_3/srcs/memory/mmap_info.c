#include "mmap.h"
#include "mmap_inline.h"

static mem_info_t add_mem_infos_by_size(mmap_t* mmap, uint32_t size);
static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t size, mem_info_t mem_infos);

/* =============================== get mmaps infos ========================== */

void get_mmap_infos(mmap_t* mmap, mem_info_t* mem_infos) {
	for (uint32_t i = 0; i <= MMAP_MAX_SIZE; ++i) {
		uint32_t size = MMAP_MAX_SIZE - i;
		mem_infos[size] = add_mem_infos_by_size(mmap, size);
	}
}

static mem_info_t add_mem_infos_by_size(mmap_t* mmap, uint32_t size) {
	uint32_t   len = get_bytes_nb(mmap, size);
	mem_info_t mem_info = {0, 0, 0};
	if (size > mmap->max_size) {
		return (mem_info);
	}
	for (uint32_t i = 0; i < len; ++i) {
		uint8_t byte = mmap->mmap[size][i];
		mem_info = add_mem_infos_by_byte(byte, size, mem_info);
	}
	return (mem_info);
}

static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t size, mem_info_t mem_info) {
	for (uint32_t offset = 0; offset < 8; offset += 2) {
		uint32_t status = get_byte_status(byte, offset);
		switch (status) {
			case MMAP_FREE:
				mem_info.free += 1 << size;
				mem_info.total += 1 << size;
				break;
			case MMAP_USED:
				mem_info.used += 1 << size;
				mem_info.total += 1 << size;
				break;
			case MMAP_USED_RONLY:
				mem_info.used += 1 << size;
				mem_info.total += 1 << size;
				break;
			default:
				break;
		}
	}
	return (mem_info);
}
