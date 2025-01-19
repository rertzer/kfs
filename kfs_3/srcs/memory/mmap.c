#include "mmap.h"
#include "kernel.h"

static inline uint32_t get_byte(uint32_t chunk_index);
static inline uint32_t get_offset(uint32_t chunk_index);
static inline uint8_t  set_byte_status(uint8_t byte, chunk_t chunk);
static inline uint8_t  get_byte_status(uint8_t byte, uint8_t offset);
static void*		   memory_adder(void* addr, uint32_t len);
static bool			   memory_overflow(void* addr, uint32_t len);
static void		freeze_pages(mmap_t* mmap, uint32_t start_page_index, uint32_t end_page_index);
static void		freeze_page(mmap_t* mmap, uint32_t page_index, uint32_t size);
static uint32_t get_page_range_chunk_size(uint32_t start_page_index, uint32_t end_page_index);
static void		set_all_memory_free(mmap_t* mmap);

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
	uint32_t offset = ONE_PAGE_BYTES_NB;

	// printk("map 0 %u %08x\n", offset, (*mmap)[0]);
	for (uint32_t i = 1; i <= MMAP_MAX_SIZE; ++i) {
		(*mmap)[i] = (*mmap)[i - 1] + offset;
		// printk("map %u %u %08x\n", i, offset, (*mmap)[i]);
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

void split_chunk(mmap_t* mmap, chunk_t);

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
	if (chunk.size == 0) {
		return;
	}
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
	// parent
	chunk.status = MMAP_UNAVAILABLE;
	set_chunk_status(mmap, chunk);
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
