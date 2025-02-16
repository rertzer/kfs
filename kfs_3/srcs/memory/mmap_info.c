#include "mmap.h"
#include "mmap_inline.h"

static mem_info_t add_mem_infos_by_shift(mmap_t* mmap, uint32_t shift);
static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t shift, mem_info_t mem_infos);

/* =============================== get mmaps infos ========================== */

void get_mmap_infos(mmap_t* mmap, mem_info_t* mem_infos) {
	for (uint32_t i = 0; i <= MMAP_MAX_SHIFT; ++i) {
		uint32_t shift = MMAP_MAX_SHIFT - i;
		mem_infos[shift] = add_mem_infos_by_shift(mmap, shift);
	}
}

static mem_info_t add_mem_infos_by_shift(mmap_t* mmap, uint32_t shift) {
	uint32_t   len = get_bytes_nb(mmap, shift);
	mem_info_t mem_info = {0, 0, 0};
	if (shift > mmap->max_shift) {
		return (mem_info);
	}
	for (uint32_t i = 0; i < len; ++i) {
		uint8_t byte = mmap->mmap[shift][i];
		mem_info = add_mem_infos_by_byte(byte, shift, mem_info);
	}
	return (mem_info);
}

static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t shift, mem_info_t mem_info) {
	for (uint32_t offset = 0; offset < 8; offset += 2) {
		uint32_t status = get_byte_status(byte, offset);
		switch (status) {
			case MMAP_FREE:
				mem_info.free += 1 << shift;
				mem_info.total += 1 << shift;
				break;
			case MMAP_USED:
				mem_info.used += 1 << shift;
				mem_info.total += 1 << shift;
				break;
			case MMAP_USED_RONLY:
				mem_info.used += 1 << shift;
				mem_info.total += 1 << shift;
				break;
			default:
				break;
		}
	}
	return (mem_info);
}
