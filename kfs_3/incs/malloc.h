#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>
#include <stdint.h>
#include "kernel.h"
#include "memory.h"

typedef struct s_block  t_block;
typedef struct s_page   t_page;

struct s_block {
    t_block* prev_block;
    size_t  size;
    t_page*  page;
    t_block* next;
    t_block* prev;
};

struct s_page {
    t_page          *next;
    t_page          *prev;
    size_t          block_count;
    struct s_block  free;
};

#define SIZEOF_BLOCK    sizeof(struct s_block)
#define SIZEOF_PAGE     sizeof(struct s_page)

#define UNFLAG(size) (size_t)(size & ~0b11)

#define FREE 1

#define ALIGNMENT 8
#define MAX_BINS    128
#define MAX_FAST_SIZE 64

typedef struct s_alloc {
    t_block bins[MAX_BINS];
    t_page  pages;
} t_alloc;

#define SIZEOF_METADATA_PAGE (SIZEOF_PAGE + SIZEOF_BLOCK)

typedef void* (*t_fp_mbook)(size_t size);
typedef void (*t_fp_munbook)(void const* const v_addr);


t_block *get_next_block(t_block *block);
t_block *remove_block_from_bins(t_block *block);

// alloc / release
t_page*   request_page(size_t size, t_alloc *alloc, t_fp_mbook mmap);

t_block* fragment_block(t_block* block, size_t size);

void    release_page(t_page* page, t_fp_munbook mmap);


/* Utils */
t_block *request_block(size_t size, t_alloc *alloc, t_fp_mbook mmap);
void    release_block(t_block* block, t_alloc *alloc, t_fp_munbook unmmap);

t_block* coalesce_block(t_block* block);
void    *addr_offset(void *addr, size_t offset);


void    __free(void* addr, t_alloc *alloc, t_fp_munbook unmmap);
void*   __alloc(size_t size, t_alloc *alloc, t_fp_mbook mmap);
size_t  get_alloc_size(void* addr);

void    show_alloc_mem(t_alloc* alloc);

// Implementation
void*   kvmalloc(size_t size);
void    kvfree(void* addr);
void*   kpmalloc(size_t size);
void    kpfree(void* addr);
void*   pmalloc(size_t size);
void    pfree(void* addr);
void*   vmalloc(size_t size);
void    vfree(void* addr);

// Test

typedef struct s_debug_where {
    char*   function_name;
    char*   file_name;
    int     line_number;
} t_debug_where;

#define DEBUG_WHERE (t_debug_where){        \
    .function_name  = (char*)__func__,      \
    .file_name      = (char*)__FILE__,      \
    .line_number    = (int)__LINE__}

#include <stdarg.h>

#ifndef MAX_TEST_SUITE
# define MAX_TEST_SUITE 128
#endif

typedef struct s_test_suite t_test_suite;
typedef void    (*t_fp_test_suite)(t_test_suite *);

typedef enum e_memory_type  {
    PHYSICAL, VIRTUAL,
    MAX_MEMORY_TYPE
} t_memory_type;
 
typedef struct s_test_expectation {
    int diff[MAX_MEMORY_TYPE];
} t_test_expectation;

typedef struct s_expectation t_expectation;
typedef void (*t_ft_report)(bool expected, bool current_diff, t_expectation*);

typedef struct s_set_test {
    void (*alloc)(size_t alloc_size);
    void (*free)(void);
    void (*write)(size_t index, const char* str);
} t_set_test;

t_set_test test(
    t_debug_where       where,
    t_test_suite        *test_suite,
    size_t              ptr_index,
    t_test_expectation  expt);

#define SET_TEST(test_suite, ptr_index, ...) test( \
    DEBUG_WHERE, \
    test_suite, \
    ptr_index, \
    (t_test_expectation){.diff = {__VA_ARGS__}})

__attribute__ ((sentinel))
void test_malloc(t_ft_report report, ...);

#endif //MALLOC_H