#include "malloc.h"
#include "builtin.h"
#include "keycode.h"
#include "memory.h"

typedef enum e_tests {
    USER_PHYSICAL,
    USER_VIRTUAL, 
    ROOT_PHYSICAL,
    ROOT_VIRTUAL,
    MAX_TESTS,
} t_tests_case;

static t_allocator_test allocators_tests [MAX_TESTS] = {
    [USER_PHYSICAL] = {pmalloc,     pfree,  get_memory_infos},
    [USER_VIRTUAL]  = {vmalloc,     vfree,  get_user_virtual_memory_infos},
    [ROOT_PHYSICAL] = {kpmalloc,    kpfree, get_memory_infos},
    [ROOT_VIRTUAL]  = {kvmalloc,    kvfree, get_supervisor_virtual_memory_infos},
};

typedef struct s_allocator_test {
    void*       (*alloc)(size_t);
    void        (*free)(void*);
    void        (*get_memory_info)(mem_info_t*);
    size_t      alloc_size;
    size_t      write_at;
    uint8_t*    addr;
} t_allocator_test;

static void wrap_get_mem_info(t_tests_case test_case, void (*fp)(t_tests_case)) {
    mem_info_t  mem_infos[2][MAX_TESTS][MMAP_MAX_SHIFT + 1];
    t_allocator_test *test = &allocators_tests[test_case];

    for (size_t i = 0; i < MAX_TESTS; i++) {
        test->get_memory_info(mem_infos[0][i]);
    }
    fp(test_case);
    for (size_t i = 0; i < MAX_TESTS; i++) {
        test->get_memory_info(mem_infos[1][i]);
        // check diff
    }
}

static void test_alloc(t_tests_case test_case) {
    t_allocator_test *test = &allocators_tests[test_case];

    test->addr = test->alloc(test->alloc_size);
}

static void test_free(t_tests_case test_case) {
    t_allocator_test *test = &allocators_tests[test_case];

    test->free(test->addr);
}

static void test_write_in_memory(t_tests_case test_case) {
    t_allocator_test *test = &allocators_tests[test_case];

    test->addr[test->write_at] = 'y';
}

void    test_allocator(t_tests_case test_case) {
    t_allocator_test *test = &allocators_tests[test_case];

    test->alloc_size = 256;
    wrap_get_mem_info(test_case, test_alloc);
    test->write_at = 0;
    wrap_get_mem_info(test_case, test_write_in_memory);

    printk("toto = %s\n", test->addr);
    wrap_get_mem_info(test_case, test_free);
}

static void get_user_virtual_memory_infos(mem_info_t *mem_infos) {
	get_virtual_memory_infos(mem_infos, USER_LEVEL);
}

static void get_supervisor_virtual_memory_infos(mem_info_t *mem_infos) {
	get_virtual_memory_infos(mem_infos, SUPERVISOR_LEVEL);
}

void    test_malloc(void)
{
    for (size_t i = 0; i < MAX_TESTS; i++) {
        test_allocator(i);
    }
}
