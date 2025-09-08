#include "mmap_inline.h"
#include "mmap.h"

extern inline uint32_t get_bytes_nb(mmap_t const* const mmap, uint32_t const shift);
extern inline uint8_t  get_byte_status(uint8_t byte, uint8_t offset);
extern inline uint32_t get_local_page_index(mmap_t const* const mmap, uint32_t const page_index);
extern inline bool	   valid_page_index(mmap_t const* const mmap, uint32_t const page_index);
extern inline uint32_t get_byte(uint32_t chunk_index);
extern inline uint32_t get_byte_size(uint32_t chunk_shift);
extern inline uint32_t get_page_size(uint32_t chunk_shift);
extern inline uint32_t get_offset(uint32_t chunk_index);
extern inline uint32_t get_chunk_index(chunk_t chunk);
extern inline uint32_t get_page_index(void const* const addr);
