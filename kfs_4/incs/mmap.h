#ifndef MMAP_H
#define MMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 2 bits per page:
 *  00 not available
 *  01 freezed (multiboot infos, etc.)
 *  10 used block
 *  11 free block
 *
 * */

#define MMAP_MAX_SHIFT 15
#define MMAP_UNAVAILABLE 0
#define MMAP_USED_RONLY 1
#define MMAP_USED 2
#define MMAP_FREE 3

#define PAGES_PER_BYTE 4
#define PAGES_PER_BYTE_SHIFT 2
#define BITS_PER_CHUNK 2
#define BITS_PER_PAGE_SHIFT 1
#define MMAP_PAGE_MASK 3
#define MMAP_NOT_FOUND_OFFSET 8

#define MMAP_MAX_KBYTE_SIZE 4194304	 // 4 Gb

typedef struct {
	uint32_t start_index;
	uint32_t end_index;
	uint32_t max_shift;
	uint32_t bytes_nb;
	uint8_t* mmap[MMAP_MAX_SHIFT + 1];
} mmap_t;

typedef struct chunk {
	uint32_t shift;
	uint32_t byte;
	uint32_t offset;
	uint32_t status;
} chunk_t;

typedef struct mem_info {
	uint32_t free;
	uint32_t used;
	uint32_t total;
} mem_info_t;

typedef struct chunk_info {
	void const* addr;
	bool		valid;
	uint32_t	shift;
	uint32_t	page_nb;
	uint32_t	status;
} chunk_info_t;

void		 init_mmap(mmap_t* mmap, uint8_t* start, uint8_t* memory_start, uint32_t memory_size);
void		 set_all_memory_free(mmap_t* mmap);
uint32_t	 get_size_by_address(mmap_t* mmap, void const* const addr);
chunk_info_t get_chunk_info(mmap_t* mmap, void const* const addr);
chunk_t		 get_chunk(mmap_t* mmap, uint32_t page_index);
uint32_t	 get_chunk_status(mmap_t* mmap, chunk_t chunk);
void		 set_chunk_status(mmap_t* mmap, chunk_t chunk);
void		 book_memory(mmap_t* mmap, uint8_t* addr, uint32_t len, uint32_t status);
void		 get_mmap_infos(mmap_t* mmap, mem_info_t* mem_infos);
chunk_t		 get_free_chunk(mmap_t* mmap, uint32_t shift);
void*		 get_chunk_address(mmap_t* mmap, chunk_t chunk);
uint8_t		 free_by_address(mmap_t* mmap, void const* const addr);
void		 split_chunk(mmap_t* mmap, chunk_t chunk);

#endif
