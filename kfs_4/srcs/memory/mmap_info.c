#include "mmap.h"
#include "mmap_inline.h"

static mem_info_t add_mem_infos_by_shift(mmap_t const* const mmap, uint32_t const shift);
static mem_info_t add_mem_infos_by_byte(uint8_t const byte, uint32_t const shift, mem_info_t mem_infos);

void get_mmap_infos(mmap_t const* const mmap, mem_info_t* const mem_infos) {
	for (uint32_t shift = 0; shift <= MMAP_MAX_SHIFT; ++shift) {
		mem_infos[shift] = add_mem_infos_by_shift(mmap, shift);
	}
}

static mem_info_t add_mem_infos_by_shift(mmap_t const* const mmap, uint32_t const shift) {
	uint32_t   len = get_bytes_nb(mmap, shift);
	mem_info_t mem_info = {0, 0, 0};

	if (shift > mmap->max_shift) {
		return (mem_info);
	}

	for (uint32_t byte_index = 0; byte_index < len; ++byte_index) {
		uint8_t byte = mmap->mmap[shift][byte_index];
		mem_info = add_mem_infos_by_byte(byte, shift, mem_info);
	}

	return (mem_info);
}

static mem_info_t add_mem_infos_by_byte(uint8_t const byte, uint32_t const shift, mem_info_t mem_info) {
	for (uint32_t offset = 0; offset < 8; offset += 2) {
		switch (get_byte_status(byte, offset)) {
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
