#include "malloc.h"
#include "builtin.h"
#include "keycode.h"
#include "memory.h"

/* User and Memory kind */
typedef struct s_ctx {
    enum { USER, ROOT, MAX_USER_TYPE }          user;
    enum { PHYSICAL, VIRTUAL, MAX_MEMORY_TYPE } memory;
} t_ctx;
static const char   *user_type_names[MAX_USER_TYPE] = {"USER", "ROOT"};
static const char   *memory_type_names[MAX_MEMORY_TYPE] = {"PHYSICAL", "VIRTUAL"};

/* test type */
typedef struct s_test t_test;

/*
 * Allocator callbacks
*/
typedef struct s_allocator_callbacks {
    void*       (*alloc)(size_t);
    void        (*dealloc)(void*);
    void        (*get_memory_info)(mem_info_t*);
} t_allocator_callbacks;

static void get_user_virtual_memory_infos(mem_info_t *mem_infos) {
	get_virtual_memory_infos(mem_infos, USER_LEVEL);
}

static void get_supervisor_virtual_memory_infos(mem_info_t *mem_infos) {
	get_virtual_memory_infos(mem_infos, SUPERVISOR_LEVEL);
}

static t_allocator_callbacks    allocator(t_ctx ctx) {
    static t_allocator_callbacks allocators_tests [MAX_USER_TYPE][MAX_MEMORY_TYPE] = {
        [USER][PHYSICAL] = {
            .alloc              = pmalloc,
            .dealloc            = pfree,
            .get_memory_info    = get_memory_infos,
        },
        [USER][VIRTUAL]  = {
            .alloc              = vmalloc,
            .dealloc            = vfree,
            .get_memory_info    = get_user_virtual_memory_infos
        },
        [ROOT][PHYSICAL] = {
            .alloc              = kpmalloc,
            .dealloc            = kpfree,
            .get_memory_info    = get_memory_infos
        },
        [ROOT][VIRTUAL]  = {
            .alloc              = kvmalloc,
            .dealloc            = kvfree,
            .get_memory_info    = get_supervisor_virtual_memory_infos
        },
    };
    return (allocators_tests[ctx.user][ctx.memory]);
}

/*
 * Data struct as helper
*/

struct s_expectation {
    struct s_memory_report {
        mem_info_t  before[MAX_USER_TYPE][MAX_MEMORY_TYPE][MMAP_MAX_SHIFT + 1];
        mem_info_t  after[MAX_USER_TYPE][MAX_MEMORY_TYPE][MMAP_MAX_SHIFT + 1];
    }       memory_report;
    t_ctx   ctx;
    t_ctx   ctx_report;
    int     diff[MAX_USER_TYPE][MAX_MEMORY_TYPE];
    t_test  *current_test;
};

/*
 * Test suite
*/

struct s_test {
    enum e_test_kind {
        ALLOC, DEALLOC, WRITE_IN_MEMORY,
        MAX_TEST_KIND
    } kind;
    union u_data {
        size_t  alloc_size;
        void    **addr;
        struct {
            size_t      at;
            char  *data;
        } write;
    } data;
    t_debug_where where;
};

struct s_test_suite {
    void*   addrs[MAX_TEST_SUITE];
    t_test  tests[MAX_TEST_SUITE];
    size_t  count;
};

static void test_suite_append_test(t_test_suite *test_suite, t_test new_test) {
    if (!test_suite || test_suite->count == MAX_TEST_SUITE) {
        return;
    }
    new_test.data.addr = &test_suite->addrs[(size_t)new_test.data.addr];
    ft_memcpy(&test_suite->tests[test_suite->count], &new_test, sizeof(t_test));
    test_suite->count += 1;
}

static t_test       test_to_set;
static t_test_suite *current_test_suite = NULL;

static void test_alloc(size_t alloc_size) {
    test_to_set.kind = ALLOC;
    test_to_set.data.alloc_size = alloc_size;
    test_suite_append_test(current_test_suite, test_to_set);
}

static void test_free(void) {
    test_to_set.kind = DEALLOC;
    test_suite_append_test(current_test_suite, test_to_set);
}

static void test_write(size_t write_at, const char *str) {
    test_to_set.kind = WRITE_IN_MEMORY;
    test_to_set.data.write.at = write_at;
    test_to_set.data.write.data = (char*)str;
    test_suite_append_test(current_test_suite, test_to_set);
}

t_set_test test(t_debug_where where, t_test_suite* test_suite, size_t ptr_index) {
    static const t_set_test set_test = {
        .alloc  = test_alloc,
        .free   = test_free,
        .write  = test_write,
    };
    current_test_suite = test_suite;
    test_to_set.data.addr = (void**)ptr_index;
    test_to_set.where = where;
    return (set_test);
}

///////////

typedef void (*t_fp_on_case)(t_ctx, void*);

static void for_all_case(t_fp_on_case on_case, void* data) {
    for (size_t user_type = 0; user_type < MAX_USER_TYPE; user_type++) {
        for(size_t memory_type = 0; memory_type < MAX_MEMORY_TYPE; memory_type++) {
            on_case((t_ctx){user_type, memory_type}, data);
        }
    }
}

static void call_allocator(t_ctx ctx, void* data) {
    t_test*  test = (t_test*)data;
    test->data.addr = allocator(ctx).alloc(test->data.alloc_size);
}

static void call_deallocator(t_ctx ctx, void* data) {
    t_test*  test = (t_test*)data;
    allocator(ctx).dealloc(test->data.addr);
}

static void call_write_in_memory(t_ctx ctx, void* data) {
    (void)ctx;
    t_test*  test = (t_test*)data;
    ft_memcpy(
        test->data.addr[test->data.write.at],
        test->data.write.data,
        strlen(test->data.write.data));
}

static void call_get_memory_info(t_ctx ctx, void* data) {
    mem_info_t  ***mem_info = data;
    t_allocator_callbacks   callback;

    callback = allocator(ctx);
    printk("%s %s %x\n", 
        user_type_names[ctx.user],
        memory_type_names[ctx.memory],
        *mem_info);
    
	press_any();
    callback.get_memory_info((mem_info_t*)&mem_info[ctx.user][ctx.memory]);
}

static t_ft_report report_expectation_callback = NULL;
static void report_on_unexpected_memory(t_ctx ctx, void* data) {
    int             expected_diff;
    int             current_diff;
    int             is_unexpected;
    t_expectation   *expt = data;

    expt->ctx_report = ctx;
    expected_diff = expt->diff[ctx.user][ctx.memory];
    current_diff = ft_memcmp(
        &expt->memory_report.before[ctx.user][ctx.memory],
        &expt->memory_report.after[ctx.user][ctx.memory],
        sizeof(mem_info_t) * (MMAP_MAX_SHIFT + 1));
    is_unexpected = (!current_diff != !expected_diff);
    report_expectation_callback(!is_unexpected, expt);
}

///////////

static void check(t_expectation *expt) {
    static t_fp_on_case call[MAX_TEST_KIND] = {
        [ALLOC]             = call_allocator,
        [DEALLOC]           = call_deallocator,
        [WRITE_IN_MEMORY]   = call_write_in_memory,
    };

    for_all_case(call_get_memory_info, (mem_info_t***)&expt->memory_report.before);
	press_any();
    call[expt->current_test->kind](expt->ctx, expt->current_test);
    for_all_case(call_get_memory_info, expt->memory_report.after);

    for_all_case(report_on_unexpected_memory, &expt);
}

static void set_fatal_expectation(t_expectation *expt) {
    (void)expt;
/*
    if(diff) {
        if (ctx.user_type != test->current_ctx.user_type) {
            // BAD_MEMORY_ACROSS_USERS
        }
    } else {
        if (ctx.user_type == test->current_ctx.user_type) {
            report;
        }
    }
*/
}

static void test_allocator(t_ctx ctx, void* data) {
    t_expectation expt = {.ctx = ctx};
    t_test_suite  *tests = data;


    set_fatal_expectation(&expt);
    for (size_t i = 0; i < tests->count; i++) {
        expt.current_test = &tests->tests[i];
        check(&expt);       
    }
}

static void show_expectation(bool is_expected, t_expectation *expt) {
    t_ctx   ctx_report;
    if (is_expected == true) return;
    ctx_report = expt->ctx_report;
    printk("On [%s][%s] found unexpected diff against [%s][%s] memory infos\n",
        user_type_names[expt->ctx.user],
        memory_type_names[expt->ctx.memory],
        user_type_names[ctx_report.user],
        memory_type_names[ctx_report.memory]
    );
	press_any();
	print_mem_infos(expt->memory_report.before[ctx_report.user][ctx_report.memory]);
    press_any();
	print_mem_infos(expt->memory_report.after[ctx_report.user][ctx_report.memory]);
	press_any();
}

void    test_malloc(t_ft_report report, ...)
{
	va_list         l_args;
    t_fp_test_suite current_test;
    t_test_suite    tests;

    report_expectation_callback = report ? report : show_expectation;
	va_start(l_args, report);
    current_test = va_arg(l_args, t_fp_test_suite);
    while (current_test) {
        tests.count = 0;
        current_test(&tests);
        for_all_case(test_allocator, &tests);
        current_test = va_arg(l_args, t_fp_test_suite);
    }
	va_end(l_args);
}
