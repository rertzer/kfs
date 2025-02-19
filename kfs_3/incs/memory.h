#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mmap.h"
#define PAGE_SIZE 4096
#define KERNEL_SIZE 1024 * PAGE_SIZE
#define VIRTUAL_MEMORY_KIB_SIZE 4194304
#define USER_VIRTUAL_MEMORY_KIB_SIZE 3145728
#define KERNEL_VIRTUAL_MEMORY_KIB_SIZE 1048576
#define KERNEL_VIRTUAL_MEMORY_START 3221225472
#define USER_VIRTUAL_MEMORY_START 0
#define PAGE_TABLE_ADDRESS_START
#define KERNEL_CODE_SIZE 4194304
#define USER_LEVEL 1
#define SUPERVISOR_LEVEL 0
#define READ_WRITE 1
#define READ_ONLY 0

typedef struct mmap_info {
	bool user;
	bool rw;
	bool valid;
} mmap_info_t;

// physical memory
void	 init_memory();
void	 get_memory_infos(mem_info_t* mem_infos);
void*	 k_mmap(uint32_t size);
uint32_t k_size(void* addr);
uint8_t	 k_free(void* addr);

// virtual memory
void		init_v_memory();
void		get_virtual_memory_infos(mem_info_t* mem_infos, bool level);
void*		v_mmap(uint32_t size, bool const level, bool const rw);
uint32_t	v_size(void const* const addr);
uint8_t		v_free(void const* const addr);
mmap_info_t v_mmap_check(void const* const l_address, bool const fault_level, bool const fault_rw);

// book memory
void* mbook(uint32_t const size, bool const level, bool const rw);
void  munbook(void const* const addr);
void* vmbook(uint32_t const size, bool const level, bool const rw);
void  vmunbook(void const* const addr);

// tests
void memory_test_k_mmap();
void memory_test_vmbook();
void mbook_test();
#endif
