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

#define MMAP_MAX_SIZE 15
#define MMAP_UNAVAILABLE 0
#define MMAP_FREEZED 1
#define MMAP_USED 2
#define MMAP_FREE 3

#define PAGES_PER_BYTE 4
#define BITS_PER_CHUNK 2
#define BITS_PER_PAGE_SHIFT 1
#define MMAP_PAGE_MASK 3

#define MMAP_MAX_BYTE_SIZE 4294967296  // 4 Gb
// 4 Gb give 1048576 pages, bitmap hold 4 pages per byte, gives 262144 bytes to hold 1 page
// chunks in the bitmap
#define SIZE_ONE_BYTES_NB 262144
#define MAX_SIZE_PAGE_BYTES_NB 8

typedef uint8_t* mmap_t[MMAP_MAX_SIZE + 1];

typedef struct chunk {
	uint32_t size;
	uint32_t byte;
	uint32_t offset;
	uint32_t status;
} chunk_t;

typedef struct mem_infos {
	uint32_t free;
	uint32_t used;
	uint32_t total;
} mem_info_t;

void	   init_mmap(mmap_t* mmap, uint8_t* start);
void	   set_memory_size(mmap_t* mmap, uint32_t size);
chunk_t	   make_chunk(uint32_t size, uint32_t chunk_index, uint32_t status);
chunk_t	   get_chunk(mmap_t* mmap, uint32_t page_index);
void	   set_chunk(mmap_t* mmap, chunk_t);
void	   set_chunk_status(mmap_t* mmap, chunk_t chunk);
void	   split_chunk(mmap_t* mmap, chunk_t chunk);
uint32_t   get_start_max_possible_chunk_size(uint32_t page_index);
uint32_t   get_len_max_possible_chunk_size(uint32_t len);
uint32_t   get_chunk_index(chunk_t chunk);
uint32_t   get_page_index(void* addr);
void	   freeze_memory(mmap_t* mmap, uint8_t* addr, uint32_t len);
mem_info_t get_mmap_infos(mmap_t* mmap);

#endif
