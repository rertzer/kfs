#include "malloc.h"

void*   addr_offset(void *addr, size_t offset)
{
    return (addr ? (uint8_t*)addr + offset : NULL);
}

static inline size_t align(size_t size)
{
    return ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
}

void*   __alloc(size_t size, t_alloc *alloc, t_fp_mbook mmap)
{
    t_block* block;
    
    if (size == 0) return (0);
    block = request_block(align(size), alloc, mmap);
    return (addr_offset(block, SIZEOF_BLOCK));
}

void    __free(void* addr, t_alloc *alloc, t_fp_munbook unmmap)
{
    t_block* block;
    
    if (addr == NULL) return;
    block = addr_offset(addr, -SIZEOF_BLOCK);
    release_block(coalesce_block(block), alloc, unmmap);
}

size_t  get_alloc_size(void* addr)
{
    t_block* block;

    block = addr_offset(addr, -SIZEOF_BLOCK);
    return (block->size);
}

static  void    *virtual_user_page(size_t size) {
    return (vmbook(size, USER_LEVEL, READ_WRITE));
}

static  void    *physical_user_page(size_t size) {
    return (mbook(size, USER_LEVEL, READ_WRITE));
}

static  void    *virtual_kernel_page(size_t size) {
    return (vmbook(size, SUPERVISOR_LEVEL, READ_WRITE));
}

static  void    *physical_kernel_page(size_t size) {
    return (mbook(size, SUPERVISOR_LEVEL, READ_WRITE));
}

t_alloc virual_user = {};
void*   vmalloc(size_t size)
{
    return __alloc(size, &virual_user, virtual_user_page);
}

void    vfree(void* addr)
{
    return __free(addr, &virual_user, vmunbook);
}

t_alloc physical_user = {};
void*   pmalloc(size_t size)
{
    return __alloc(size, &physical_user, physical_user_page);
}

void    pfree(void* addr)
{
    return __free(addr, &physical_user, munbook);
}

t_alloc physical_kernel = {};
void*   kpmalloc(size_t size)
{
    return __alloc(size, &physical_kernel, physical_kernel_page);
}

void    kpfree(void* addr)
{
    return __free(addr, &physical_kernel, munbook);
}

t_alloc virtual_kernel = {};
void*   kvmalloc(size_t size)
{
    return __alloc(size, &virtual_kernel, virtual_kernel_page);
}

void    kvfree(void* addr)
{
    return __free(addr, &virtual_kernel, munbook);
}