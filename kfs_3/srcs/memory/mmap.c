#include "mmap.h"
#include "kernel.h"
#include "memory.h"

static inline uint32_t get_byte(uint32_t chunk_index);
static inline uint32_t get_offset(uint32_t chunk_index);
static inline uint8_t  set_byte_status(uint8_t byte, chunk_t chunk);
static inline uint8_t  get_byte_status(uint8_t byte, uint8_t offset);
static void*		   memory_adder(void* addr, uint32_t len);
static bool			   memory_overflow(void* addr, uint32_t len);
static void		  freeze_pages(mmap_t* mmap, uint32_t start_page_index, uint32_t end_page_index);
static void		  freeze_page(mmap_t* mmap, uint32_t page_index, uint32_t size);
static uint32_t	  get_page_range_chunk_size(uint32_t start_page_index, uint32_t end_page_index);
static void		  set_all_memory_free(mmap_t* mmap);
static mem_info_t add_mem_infos_by_size(mmap_t* mmap, uint32_t size, mem_info_t mem_infos);
static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t size, mem_info_t mem_infos);
static uint32_t	  get_free_offset(uint8_t byte);
static chunk_t	  get_free_chunk_by_size(mmap_t* mmap, uint32_t size);
static chunk_t	  get_unavailable_chunk();
static uint32_t	  get_buddy_offset(uint32_t offset);
static chunk_t	  get_parent(mmap_t* mmap, chunk_t kid);

static inline uint32_t get_byte(uint32_t chunk_index) {
	return (chunk_index / PAGES_PER_BYTE);
}

static inline uint32_t get_offset(uint32_t chunk_index) {
	return ((chunk_index % PAGES_PER_BYTE) << 1);
}

static inline uint8_t set_byte_status(uint8_t byte, chunk_t chunk) {
	byte &= ~(MMAP_PAGE_MASK << chunk.offset);
	byte |= chunk.status << chunk.offset;

	return (byte);
}

static inline uint8_t get_byte_status(uint8_t byte, uint8_t offset) {
	byte >>= offset;
	byte &= MMAP_PAGE_MASK;

	return (byte);
}

uint32_t get_chunk_index(chunk_t chunk) {
	return (chunk.byte * PAGES_PER_BYTE + (chunk.offset >> BITS_PER_PAGE_SHIFT));
}

void init_mmap(mmap_t* mmap, uint8_t* start) {
	(*mmap)[0] = start;
	uint32_t offset = SIZE_ONE_BYTES_NB;

	for (uint32_t i = 1; i <= MMAP_MAX_SIZE; ++i) {
		(*mmap)[i] = (*mmap)[i - 1] + offset;
		offset >>= 1;
	}
}

void set_memory_size(mmap_t* mmap, uint32_t size) {
	// printk("memory size : %u\n", size);
	set_all_memory_free(mmap);
	uint8_t* start = (uint8_t*)size;
	uint32_t len = MMAP_MAX_BYTE_SIZE - size;
	freeze_memory(mmap, start, len);
}

uint32_t get_size_by_address(mmap_t* mmap, void* addr) {
	uint32_t size = 0;
	uint32_t page_index = get_page_index(addr);
	chunk_t	 chunk = get_chunk(mmap, page_index);
	if (chunk.status == MMAP_FREE || chunk.status == MMAP_USED) {
		size = (1 << chunk.size) * PAGE_SIZE;
	}
	return (size);
}

static void set_all_memory_free(mmap_t* mmap) {
	for (uint32_t i = 0; i < MAX_SIZE_PAGE_BYTES_NB; ++i) {
		(*mmap)[MMAP_MAX_SIZE][i] = 0xFF;
	}
}

chunk_t make_chunk(uint32_t size, uint32_t chunk_index, uint32_t status) {
	chunk_t chunk;
	chunk.size = size;
	chunk.byte = get_byte(chunk_index);
	chunk.offset = get_offset(chunk_index);
	chunk.status = status;

	return (chunk);
}

chunk_t get_chunk(mmap_t* mmap, uint32_t page_index) {
	chunk_t chunk;

	for (chunk.size = MMAP_MAX_SIZE; chunk.size != UINT32_MAX; --chunk.size) {
		uint32_t current_index = page_index >> chunk.size;
		chunk.byte = get_byte(current_index);
		chunk.offset = get_offset(current_index);
		uint32_t byte = (*mmap)[chunk.size][chunk.byte];
		chunk.status = get_byte_status(byte, chunk.offset);
		if (chunk.status != MMAP_UNAVAILABLE) {
			break;
		}
	}
	return (chunk);
}
void set_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint8_t byte = (*mmap)[chunk.size][chunk.byte];
	byte = set_byte_status(byte, chunk);
	(*mmap)[chunk.size][chunk.byte] = byte;
}

uint32_t get_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint32_t byte = (*mmap)[chunk.size][chunk.byte];
	return (get_byte_status(byte, chunk.offset));
}
uint32_t get_start_max_possible_chunk_size(uint32_t page_index) {
	uint32_t max = 0;

	for (; max < MMAP_MAX_SIZE; ++max) {
		if ((page_index & 1) == 1) {
			break;
		}
		page_index >>= 1;
	}

	return (max);
}

uint32_t get_len_max_possible_chunk_size(uint32_t len) {
	uint32_t max = MMAP_MAX_SIZE - 1;

	if (len >= (1 << MMAP_MAX_SIZE)) {
		return (MMAP_MAX_SIZE);
	}
	for (; max > 0; --max) {
		if ((len & (1 << (max))) != 0) {
			break;
		}
	}

	return (max);
}
void split_chunk(mmap_t* mmap, chunk_t chunk) {
	if (chunk.size == 0 || chunk.status != MMAP_FREE) {
		return;
	}
	// parent
	chunk.status = MMAP_UNAVAILABLE;
	set_chunk_status(mmap, chunk);
	// kids
	uint32_t index = get_chunk_index(chunk);
	// twice more entries one level lower
	index <<= 1;

	chunk_t kid_chunk;
	// left child
	kid_chunk.size = chunk.size - 1;
	kid_chunk.byte = get_byte(index);
	kid_chunk.offset = get_offset(index);
	kid_chunk.status = MMAP_FREE;
	set_chunk_status(mmap, kid_chunk);
	// right child
	kid_chunk.offset += BITS_PER_CHUNK;
	set_chunk_status(mmap, kid_chunk);
}

void fuse_chunk(mmap_t* mmap, chunk_t chunk) {
	if (chunk.size == 15 || chunk.status != MMAP_FREE) {
		return;
	}
	chunk_t buddy = get_buddy(mmap, chunk);
	if (buddy.status != MMAP_FREE) {
		return;
	}
	chunk.status = MMAP_UNAVAILABLE;
	buddy.status = MMAP_UNAVAILABLE;
	set_chunk_status(mmap, chunk);
	set_chunk_status(mmap, buddy);
	chunk = get_parent(mmap, chunk);

	set_chunk_status(mmap, chunk);
	fuse_chunk(mmap, chunk);
}

static chunk_t get_parent(mmap_t* mmap, chunk_t kid) {
	uint32_t index = get_chunk_index(kid);
	// twice less entries one higher level
	index >>= 1;
	chunk_t parent;
	parent.size = kid.size + 1;
	parent.byte = get_byte(index);
	parent.offset = get_offset(index);
	parent.status = MMAP_FREE;

	return (parent);
}

chunk_t get_buddy(mmap_t* mmap, chunk_t chunk) {
	chunk_t buddy;
	buddy.size = chunk.size;
	buddy.byte = chunk.byte;
	buddy.offset = get_buddy_offset(chunk.offset);
	buddy.status = get_chunk_status(mmap, buddy);
	return (buddy);
}

static uint32_t get_buddy_offset(uint32_t offset) {
	if ((offset < 7) && (offset % 2 == 0)) {
		if (offset % 4 == 0) {
			offset += BITS_PER_CHUNK;
		} else {
			offset -= BITS_PER_CHUNK;
		}
	} else {
		offset = MMAP_NOT_FOUND_OFFSET;
	}
	return (offset);
}

void freeze_memory(mmap_t* mmap, uint8_t* addr, uint32_t len) {
	uint32_t start_page_index = get_page_index(addr);

	uint32_t end_page_index = get_page_index(memory_adder(addr, len));
	freeze_pages(mmap, start_page_index, end_page_index);
}

uint32_t get_page_index(void* addr) {
	return ((uint32_t)addr >> 12);
}

static void freeze_pages(mmap_t* mmap, uint32_t start_page_index, uint32_t end_page_index) {
	sleep();
	uint32_t size = get_page_range_chunk_size(start_page_index, end_page_index);
	freeze_page(mmap, start_page_index, size);
	start_page_index += 1 << size;
	if (start_page_index <= end_page_index) {
		freeze_pages(mmap, start_page_index, end_page_index);
	}
}

static uint32_t get_page_range_chunk_size(uint32_t start_page_index, uint32_t end_page_index) {
	uint32_t size = get_start_max_possible_chunk_size(start_page_index);
	uint32_t len = get_len_max_possible_chunk_size(end_page_index - start_page_index + 1);
	size = size < len ? size : len;
	return (size);
}

static void* memory_adder(void* addr, uint32_t len) {
	uint32_t sum;
	if (memory_overflow(addr, len)) {
		sum = UINT32_MAX;
	} else {
		sum = (uint32_t)addr + len - 1;
	}
	return ((void*)sum);
}

static bool memory_overflow(void* addr, uint32_t len) {
	bool over = false;
	if (len > UINT32_MAX - (uint32_t)addr) {
		over = true;
	}
	return (over);
}

static void freeze_page(mmap_t* mmap, uint32_t page_index, uint32_t size) {
	chunk_t chunk = get_chunk(mmap, page_index);
	if (chunk.status == MMAP_FREEZED) {
		return;
	}
	if (chunk.size < size) {
		size -= 1;
		freeze_page(mmap, page_index, size);
		freeze_page(mmap, page_index + (1 << size), size);
	} else if (chunk.status == MMAP_FREE) {
		if (chunk.size == size) {
			chunk.status = MMAP_FREEZED;
			set_chunk_status(mmap, chunk);
		} else {
			split_chunk(mmap, chunk);
			freeze_page(mmap, page_index, size);
		}
	}
}

mem_info_t get_mmap_infos(mmap_t* mmap) {
	mem_info_t mem_infos = {0, 0, 0};

	for (uint32_t i = 0; i <= MMAP_MAX_SIZE; ++i) {
		uint32_t size = MMAP_MAX_SIZE - i;
		mem_infos = add_mem_infos_by_size(mmap, size, mem_infos);
	}
	return (mem_infos);
}

static mem_info_t add_mem_infos_by_size(mmap_t* mmap, uint32_t size, mem_info_t mem_infos) {
	uint32_t len = (uint32_t)SIZE_ONE_BYTES_NB >> size;

	for (uint32_t i = 0; i < len; ++i) {
		uint8_t byte = (*mmap)[size][i];
		mem_infos = add_mem_infos_by_byte(byte, size, mem_infos);
	}
	return (mem_infos);
}

static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t size, mem_info_t mem_infos) {
	for (uint32_t offset = 0; offset < 8; offset += 2) {
		uint32_t status = get_byte_status(byte, offset);
		switch (status) {
			case MMAP_FREE:
				mem_infos.free += 1 << size;
				mem_infos.total += 1 << size;
				break;
			case MMAP_USED:
				mem_infos.used += 1 << size;
				mem_infos.total += 1 << size;
				break;
			default:
				break;
		}
	}
	return (mem_infos);
}

chunk_t get_free_chunk(mmap_t* mmap, uint32_t size) {
	if (size > 15) {
		return get_unavailable_chunk();
	}
	chunk_t chunk = get_free_chunk_by_size(mmap, size);
	if (chunk.status != MMAP_FREE) {
		chunk = get_free_chunk(mmap, size + 1);

		split_chunk(mmap, chunk);
		chunk = get_free_chunk_by_size(mmap, size);
	}
	return (chunk);
}

static chunk_t get_free_chunk_by_size(mmap_t* mmap, uint32_t size) {
	uint32_t len = (uint32_t)SIZE_ONE_BYTES_NB >> size;

	chunk_t chunk;
	chunk.size = size;
	chunk.status = MMAP_UNAVAILABLE;

	for (uint32_t i = 0; i < len; ++i) {
		uint8_t	 byte = (*mmap)[size][i];
		uint32_t offset = get_free_offset(byte);
		if (offset != MMAP_NOT_FOUND_OFFSET) {
			chunk.byte = i;
			chunk.offset = offset;
			chunk.status = MMAP_FREE;
			break;
		}
	}
	return (chunk);
}

static uint32_t get_free_offset(uint8_t byte) {
	uint32_t offset;

	for (offset = 0; offset < 8; offset += 2) {
		uint32_t status = get_byte_status(byte, offset);
		if (status == MMAP_FREE) {
			break;
		}
	}
	return (offset);
}

void* get_chunk_address(chunk_t chunk) {
	uint32_t page_index = get_chunk_index(chunk) << chunk.size;
	return ((void*)(page_index * PAGE_SIZE));
}

static chunk_t get_unavailable_chunk() {
	chunk_t chunk;
	chunk.size = 0;
	chunk.byte = 0;
	chunk.offset = MMAP_NOT_FOUND_OFFSET;
	chunk.status = MMAP_UNAVAILABLE;
	return (chunk);
}

uint8_t free_by_address(mmap_t* mmap, void* addr) {
	uint8_t	 ret = 0;
	uint32_t page_index = get_page_index(addr);

	chunk_t chunk = get_chunk(mmap, page_index);
	if (chunk.status == MMAP_USED) {
		chunk.status = MMAP_FREE;
		set_chunk_status(mmap, chunk);
		fuse_chunk(mmap, chunk);
	} else {
		ret = 1;
	}

	return (ret);
}
