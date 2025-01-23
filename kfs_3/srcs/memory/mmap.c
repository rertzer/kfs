#include "mmap.h"
#include "kernel.h"
#include "memory.h"

static inline uint32_t get_byte(uint32_t chunk_index);
static inline uint32_t get_offset(uint32_t chunk_index);
static inline uint8_t  set_byte_status(uint8_t byte, chunk_t chunk);
static inline uint8_t  get_byte_status(uint8_t byte, uint8_t offset);
static void*		   memory_adder(void* addr, uint32_t len);
static bool			   memory_overflow(void* addr, uint32_t len);
static void			   book_pages(mmap_t*  mmap,
								  uint32_t start_page_index,
								  uint32_t end_page_index,
								  uint32_t status);
static void			   book_page(mmap_t* mmap, uint32_t page_index, uint32_t size, uint32_t status);
static uint32_t	  get_page_range_chunk_size(uint32_t start_page_index, uint32_t end_page_index);
static mem_info_t add_mem_infos_by_size(mmap_t* mmap, uint32_t size);
static mem_info_t add_mem_infos_by_byte(uint8_t byte, uint32_t size, mem_info_t mem_infos);
static uint32_t	  get_free_offset(uint8_t byte);
static chunk_t	  get_free_chunk_by_size(mmap_t* mmap, uint32_t size);
static chunk_t	  get_unavailable_chunk();
static uint32_t	  get_buddy_offset(uint32_t offset);
static chunk_t	  get_parent(mmap_t* mmap, chunk_t kid);
static uint32_t	  get_pages_len(mmap_t* mmap);
static bool		  valid_page_index(mmap_t* mmap, uint32_t page_index);
static bool		  valid_max_chunk_aligned_page_index(uint32_t page_index);
static uint32_t	  get_size_bytes_len(mmap_t* mmap, uint32_t size);
static uint32_t	  get_local_page_index(mmap_t* mmap, uint32_t page_index);
static void*	  get_address_by_local_page_index(mmap_t* mmap, uint32_t local_page_index);
static void		  set_mmap_addresses(mmap_t* mmap, uint8_t* start, uint32_t memory_size);
static void		  set_memory_size(mmap_t* mmap, uint8_t* start_addr, uint32_t size);
static uint32_t	  round_up_memory_size(uint32_t size);
static uint32_t	  get_remain_len(uint32_t size);
static uint8_t*	  get_remain_start(uint8_t* memory_start, uint32_t memory_size);

static uint32_t round_up_memory_size(uint32_t size) {
	if (size % (1 << MMAP_MAX_SIZE) != 0) {
		size /= (1 << MMAP_MAX_SIZE);
		++size;
		size <<= MMAP_MAX_SIZE;
	}
	return (size);
}

static void* get_address_by_local_page_index(mmap_t* mmap, uint32_t local_page_index) {
	return ((void*)((local_page_index + mmap->start_index) * PAGE_SIZE));
}

static uint32_t get_local_page_index(mmap_t* mmap, uint32_t page_index) {
	return (page_index - mmap->start_index);
}
LE CODE BLOC LA IL FAUT AVOIR UNE SIZE MAX DYNAMIC		   A 1 /
	4 min DE LA PUISSANCE SUP ARRONDIE CHANGER LA FONCTION D'ARRONDI static uint32_t
	get_size_bytes_len(mmap_t* mmap, uint32_t size) {
	printk("pages len %u %u\n", size, get_pages_len(mmap));
	return ((get_pages_len(mmap) / (uint32_t)PAGES_PER_BYTE) >> size);
}

static bool valid_max_chunk_aligned_page_index(uint32_t page_index) {
	bool valid = true;
	printk("%u %u %u\n", page_index, 1 << MMAP_MAX_SIZE, page_index % (1 << MMAP_MAX_SIZE));
	if (page_index % (1 << MMAP_MAX_SIZE) != 0) {
		valid = false;
	}
	return (valid);
}

static bool valid_page_index(mmap_t* mmap, uint32_t page_index) {
	bool valid = true;
	if (page_index < mmap->start_index || page_index > mmap->end_index) {
		valid = false;
	}
	return (valid);
}

static uint32_t get_pages_len(mmap_t* mmap) {
	return (mmap->end_index - mmap->start_index + 1);
}

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

uint32_t get_page_index(void* addr) {
	return ((uint32_t)addr >> 12);
}

void* get_chunk_address(mmap_t* mmap, chunk_t chunk) {
	uint32_t local_page_index = get_chunk_index(chunk) << chunk.size;
	return (get_address_by_local_page_index(mmap, local_page_index));
}

// memory size KIB_in Kib
void init_mmap(mmap_t* mmap, uint8_t* start, uint8_t* memory_start, uint32_t memory_size) {
	set_memory_size(mmap, memory_start, memory_size);
	set_mmap_addresses(mmap, start, memory_size);
	set_all_memory_free(mmap);

	book_memory(mmap, get_remain_start(memory_start, memory_size), get_remain_len(memory_size),
				MMAP_UNAVAILABLE);
}

static uint8_t* get_remain_start(uint8_t* memory_start, uint32_t memory_size) {
	return (memory_start + memory_size * 1024 - 1);
}

static uint32_t get_remain_len(uint32_t size) {
	return (round_up_memory_size(size) - size);
}

// size in Kib
static void set_memory_size(mmap_t* mmap, uint8_t* start_addr, uint32_t size) {
	// printk("memory size : %u Kib\n", size);
	mmap->start_index = get_page_index(start_addr);
	size = round_up_memory_size(size);
	// 4 Kib per page
	mmap->end_index = mmap->start_index + (size >> 2) - 1;
	printk("start %u end %u len %u\n", mmap->start_index, mmap->end_index, get_pages_len(mmap));

	if (valid_max_chunk_aligned_page_index(mmap->start_index) == false) {
		printk("memory error: memory start must be 2^15 bytes aligned\n");
		printk("Panic Now!");
		godot();
	}
}

static void set_mmap_addresses(mmap_t* mmap, uint8_t* start, uint32_t memory_size) {
	mmap->mmap[0] = start;
	uint32_t offset = BYTES_PER_MB * (memory_size >> 10);

	for (uint32_t i = 1; i <= MMAP_MAX_SIZE; ++i) {
		mmap->mmap[i] = mmap->mmap[i - 1] + offset;
		offset >>= 1;
	}
}

uint32_t get_size_by_address(mmap_t* mmap, void* addr) {
	uint32_t size = 0;
	uint32_t local_page_index = get_local_page_index(mmap, get_page_index(addr));
	chunk_t	 chunk = get_chunk(mmap, local_page_index);
	if (chunk.status == MMAP_FREE || chunk.status == MMAP_USED || chunk.status == MMAP_USED_WONLY) {
		size = (1 << chunk.size) * PAGE_SIZE;
	}
	return (size);
}

void set_all_memory_free(mmap_t* mmap) {
	printk("size by len %u\n", get_size_bytes_len(mmap, MMAP_MAX_SIZE));
	for (uint32_t byte_index = 0; byte_index < get_size_bytes_len(mmap, MMAP_MAX_SIZE);
		 ++byte_index) {
		printk("setting free %u", byte_index);
		mmap->mmap[MMAP_MAX_SIZE][byte_index] = 0xFF;
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

	if (valid_page_index(mmap, page_index) == false) {
		printk("memory: error: invalid page index %u \n", page_index);
		printk("It's time to Panic\n");
	}
	for (chunk.size = MMAP_MAX_SIZE; chunk.size != UINT32_MAX; --chunk.size) {
		uint32_t chunk_index = (get_local_page_index(mmap, page_index)) >> chunk.size;
		chunk.byte = get_byte(chunk_index);
		chunk.offset = get_offset(chunk_index);
		uint32_t byte = mmap->mmap[chunk.size][chunk.byte];
		chunk.status = get_byte_status(byte, chunk.offset);
		if (chunk.status != MMAP_UNAVAILABLE) {
			break;
		}
	}
	return (chunk);
}

void set_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint8_t byte = mmap->mmap[chunk.size][chunk.byte];
	byte = set_byte_status(byte, chunk);
	mmap->mmap[chunk.size][chunk.byte] = byte;
}

uint32_t get_chunk_status(mmap_t* mmap, chunk_t chunk) {
	uint32_t byte = mmap->mmap[chunk.size][chunk.byte];
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

void book_memory(mmap_t* mmap, uint8_t* addr, uint32_t len, uint32_t status) {
	uint32_t start_page_index = get_page_index(addr);

	printk("book memory %08x %u\n", addr, len);
	uint32_t end_page_index = get_page_index(memory_adder(addr, len));
	book_pages(mmap, start_page_index, end_page_index, status);
}

static void book_pages(mmap_t*	mmap,
					   uint32_t start_page_index,
					   uint32_t end_page_index,
					   uint32_t status) {
	uint32_t size = get_page_range_chunk_size(start_page_index, end_page_index);
	book_page(mmap, start_page_index, size, status);
	start_page_index += 1 << size;
	if (start_page_index <= end_page_index) {
		book_pages(mmap, start_page_index, end_page_index, status);
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

static void book_page(mmap_t* mmap, uint32_t page_index, uint32_t size, uint32_t status) {
	if (valid_page_index(mmap, page_index) == false) {
		printk("invalid page\n");
		return;
	}
	chunk_t chunk = get_chunk(mmap, page_index);
	if (chunk.status == status && status != MMAP_UNAVAILABLE) {
		return;
	}
	if (chunk.size < size) {
		size -= 1;
		book_page(mmap, page_index, size, status);
		book_page(mmap, page_index + (1 << size), size, status);
	} else if (chunk.status == MMAP_FREE) {
		if (chunk.size == size) {
			chunk.status = status;
			set_chunk_status(mmap, chunk);
		} else {
			split_chunk(mmap, chunk);
			book_page(mmap, page_index, size, status);
		}
	}
}

void get_mmap_infos(mmap_t* mmap, mem_info_t* mem_infos) {
	for (uint32_t i = 0; i <= MMAP_MAX_SIZE; ++i) {
		uint32_t size = MMAP_MAX_SIZE - i;
		mem_infos[size] = add_mem_infos_by_size(mmap, size);
	}
}
static mem_info_t add_mem_infos_by_size(mmap_t* mmap, uint32_t size) {
	uint32_t   len = (uint32_t)SIZE_ONE_BYTES_NB >> size;
	mem_info_t mem_info = {0, 0, 0};

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
			case MMAP_USED_WONLY:
				mem_info.used += 1 << size;
				mem_info.total += 1 << size;
				break;
			default:
				break;
		}
	}
	return (mem_info);
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
		uint8_t	 byte = mmap->mmap[size][i];
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
	if (chunk.status == MMAP_USED || chunk.status == MMAP_USED_WONLY) {
		chunk.status = MMAP_FREE;
		set_chunk_status(mmap, chunk);
		fuse_chunk(mmap, chunk);
	} else {
		ret = 1;
	}

	return (ret);
}
