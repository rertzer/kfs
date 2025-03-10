#include "malloc.h"
#include "builtin.h"
#include "keycode.h"
#include "memory.h"

#include "utils.h"	//for getting ft_memset

/* User and Memory kind */

static const char* user_type_names[MAX_USER_TYPE] = {"USER", "ROOT"};
static const char* memory_type_names[MAX_MEMORY_TYPE] = {"PHYSICAL", "VIRTUAL"};

/* test type */
typedef struct s_test t_test;

/*
 * Allocator callbacks
 */
typedef struct s_allocator_callbacks {
	void* (*alloc)(size_t);
	void (*dealloc)(void*);
	void (*get_memory_info)(mem_info_t*);
} t_allocator_callbacks;

static void get_user_virtual_memory_infos(mem_info_t* mem_infos) {
	get_virtual_memory_infos(mem_infos, USER_LEVEL);
}

static void get_supervisor_virtual_memory_infos(mem_info_t* mem_infos) {
	get_virtual_memory_infos(mem_infos, SUPERVISOR_LEVEL);
}

static t_allocator_callbacks allocator(t_ctx ctx) {
	static t_allocator_callbacks allocators_tests[MAX_USER_TYPE][MAX_MEMORY_TYPE] = {
		[USER][PHYSICAL] =
			{
				.alloc = pmalloc,
				.dealloc = pfree,
				.get_memory_info = get_memory_infos,
			},
		[USER][VIRTUAL] = {.alloc = vmalloc, .dealloc = vfree, .get_memory_info = get_user_virtual_memory_infos},
		[ROOT][PHYSICAL] = {.alloc = kpmalloc, .dealloc = kpfree, .get_memory_info = get_memory_infos},
		[ROOT][VIRTUAL] = {.alloc = kvmalloc,
						   .dealloc = kvfree,
						   .get_memory_info = get_supervisor_virtual_memory_infos},
	};
	return (allocators_tests[ctx.user][ctx.memory]);
}

/*
 * Data struct as helper
 */

typedef struct s_memory_report {
	mem_info_t report[MMAP_MAX_SHIFT + 1];
	mem_info_t total;
} t_memory_report;

typedef struct s_full_memory_report {
	t_memory_report full[MAX_USER_TYPE][MAX_MEMORY_TYPE];
} t_full_memory_report;

typedef struct s_full_historic_memory_report {
	t_full_memory_report before;
	t_full_memory_report after;
} t_full_historic_memory_report;

struct s_expectation {
	t_full_historic_memory_report memory_report;
	t_ctx						  ctx;
	t_test*						  current_test;
};

/*
 * Test suite
 */

struct s_test {
	enum e_test_kind { ALLOC, DEALLOC, WRITE_IN_MEMORY, MAX_TEST_KIND } kind;
	union u_data {
		size_t alloc_size;
		struct {
			size_t at;
			char*  data;
		} write;
	} data;
	size_t			   addr_index;
	t_test_suite*	   ref;
	t_test_expectation expt;
	t_debug_where	   where;
};

struct s_test_suite {
	void*  addrs[MAX_TEST_SUITE];
	t_test tests[MAX_TEST_SUITE];
	size_t count;
};

static void test_suite_append_test(t_test_suite* test_suite, t_test new_test) {
	if (!test_suite || test_suite->count == MAX_TEST_SUITE) {
		return;
	}
	test_suite->tests[test_suite->count] = new_test;
	test_suite->count += 1;
}

static t_test		 test_to_set;
static t_test_suite* current_test_suite = NULL;

static void test_alloc(size_t alloc_size) {
	test_to_set.kind = ALLOC;
	test_to_set.data.alloc_size = alloc_size;
	test_suite_append_test(current_test_suite, test_to_set);
}

static void test_free(void) {
	test_to_set.kind = DEALLOC;
	test_suite_append_test(current_test_suite, test_to_set);
}

static void test_write(size_t write_at, const char* str) {
	test_to_set.kind = WRITE_IN_MEMORY;
	test_to_set.data.write.at = write_at;
	test_to_set.data.write.data = (char*)str;
	test_suite_append_test(current_test_suite, test_to_set);
}

t_set_test test(t_debug_where where, t_test_suite* test_suite, size_t ptr_index, t_test_expectation expt) {
	static const t_set_test set_test = {
		.alloc = test_alloc,
		.free = test_free,
		.write = test_write,
	};
	current_test_suite = test_suite;
	test_to_set.ref = current_test_suite;
	test_to_set.addr_index = ptr_index;
	test_to_set.expt = expt;
	test_to_set.where = where;
	return (set_test);
}

///////////

typedef void (*t_fp_on_case)(t_ctx, void*);

static void for_all_case(t_fp_on_case on_case, void* data) {
	for (size_t user_type = 0; user_type < MAX_USER_TYPE; user_type++) {
		for (size_t memory_type = 0; memory_type < MAX_MEMORY_TYPE; memory_type++) {
			on_case((t_ctx){user_type, memory_type}, data);
		}
	}
}

static void call_allocator(t_ctx ctx, void* data) {
	t_test* test = (t_test*)data;
	test->ref->addrs[test->addr_index] = allocator(ctx).alloc(test->data.alloc_size);
}

static void call_deallocator(t_ctx ctx, void* data) {
	t_test* test = (t_test*)data;
	allocator(ctx).dealloc(test->ref->addrs[test->addr_index]);
}

static void call_write_in_memory(t_ctx ctx, void* data) {
	(void)ctx;
	t_test* test = (t_test*)data;
	ft_memcpy(((uint8_t*)test->ref->addrs[test->addr_index]) + test->data.write.at, test->data.write.data,
			  strlen(test->data.write.data));
}

static void call_get_memory_info(t_ctx ctx, void* data) {
	t_full_memory_report* report = data;
	allocator(ctx).get_memory_info((mem_info_t*)&report->full[ctx.user][ctx.memory].report);
}

static mem_info_t memory_report_total(t_memory_report* report) {
	ft_memset(&report->total, 0, sizeof(mem_info_t));
	for (uint32_t i = 0; i <= MMAP_MAX_SHIFT; ++i) {
		report->total.used += report->report[i].used;
		report->total.free += report->report[i].free;
		report->total.total += report->report[i].total;
	}
	return (report->total);
}

static mem_info_t full_historic_memory_report_diff(t_full_historic_memory_report* full_report, t_ctx ctx) {
	mem_info_t before = memory_report_total(&full_report->before.full[ctx.user][ctx.memory]);
	mem_info_t after = memory_report_total(&full_report->after.full[ctx.user][ctx.memory]);
	return ((mem_info_t){
		.free = after.free - before.free,
		.used = after.used - before.used,
		.total = after.total - before.total,
	});
}

static t_ft_report report_expectation_callback = NULL;
static void		   report_memory_diff(t_ctx ctx, void* data) {
	   t_expectation* expt = data;
	   report_expectation_callback(ctx, expt, expt->current_test->expt.diff[ctx.memory],
									   full_historic_memory_report_diff(&expt->memory_report, ctx).used);
}

///////////

static void check(t_expectation* expt) {
	static t_fp_on_case call[MAX_TEST_KIND] = {
		[ALLOC] = call_allocator,
		[DEALLOC] = call_deallocator,
		[WRITE_IN_MEMORY] = call_write_in_memory,
	};

	for_all_case(call_get_memory_info, &expt->memory_report.before);
	call[expt->current_test->kind](expt->ctx, expt->current_test);
	for_all_case(call_get_memory_info, &expt->memory_report.after);
	for_all_case(report_memory_diff, expt);
}

static void test_allocator(t_ctx ctx, void* data) {
	t_expectation expt = {.ctx = ctx};
	t_test_suite* tests = data;

	for (size_t i = 0; i < tests->count; i++) {
		expt.current_test = &tests->tests[i];
		check(&expt);
	}
}

static void show_expectation(t_ctx ctx, t_expectation* expt, int expected, int current_use);

void test_malloc(t_ft_report report, ...) {
	printf("test_malloc start\n");
	va_list			l_args;
	t_fp_test_suite current_test;
	t_test_suite	tests;

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
	printf("test_malloc end\n");
}

/// RPINT REPORT

static void print_header_mem_infos(int* column_size) {
	printk("%c %-6s %n%c %-13s %n%c %-13s %n%c %-13s %n%c\n", VERTICAL_LINE, "chunks", column_size + 0, VERTICAL_LINE,
		   "free", column_size + 1, VERTICAL_LINE, "used", column_size + 2, VERTICAL_LINE, "total", column_size + 3,
		   VERTICAL_LINE);
	column_size[3] -= column_size[2];
	column_size[2] -= column_size[1];
	column_size[1] -= column_size[0];
}

static void print_row_mem_infos(int* column_size, uint32_t index, t_ctx ctx, t_full_historic_memory_report* report) {
	int diff;

	diff = ft_memcmp(&report->before.full[ctx.user][ctx.memory].report[index],
					 &report->after.full[ctx.user][ctx.memory].report[index], sizeof(mem_info_t));
	if (diff == 0)
		return;

	printk("%c %-*u ", VERTICAL_LINE, column_size[0] - 3, 1 << index);

	printk("%c %-*d %-*d ", VERTICAL_LINE, column_size[1] - 3 - 7,
		   report->before.full[ctx.user][ctx.memory].report[index].free, column_size[1] - 3 - 7,
		   report->after.full[ctx.user][ctx.memory].report[index].free);

	printk("%c %-*d %-*d ", VERTICAL_LINE, column_size[2] - 3 - 7,
		   report->before.full[ctx.user][ctx.memory].report[index].used, column_size[2] - 3 - 7,
		   report->after.full[ctx.user][ctx.memory].report[index].used);

	printk("%c %-*d %-*d ", VERTICAL_LINE, column_size[3] - 3 - 7,
		   report->before.full[ctx.user][ctx.memory].report[index].total, column_size[3] - 3 - 7,
		   report->after.full[ctx.user][ctx.memory].report[index].total);

	printk("%c\n", VERTICAL_LINE);
}

static void print_full_historic_memory_report(t_ctx ctx, t_full_historic_memory_report* report) {
	int column_size[4] = {0};

	print_header_mem_infos(column_size);
	for (uint32_t i = 0; i <= MMAP_MAX_SHIFT; ++i) {
		print_row_mem_infos(column_size, i, ctx, report);
	}
}

static void print_debug(t_debug_where where) {
	printk("\"%s\" (%s:%d)", where.function_name, where.file_name, where.line_number);
};

static void print_test_context(t_ctx ctx, t_expectation* expt) {
	switch (expt->current_test->kind) {
		case ALLOC:
			printk("`ptr[%d] = alloc(%d)`", expt->current_test->addr_index, expt->current_test->data.alloc_size);
			break;
		case DEALLOC:
			printk("`free(ptr[%d])`", expt->current_test->addr_index);
			break;
		case WRITE_IN_MEMORY:
			printk("`memcpy(\"%s\", &ptr[%d] + %d)`", expt->current_test->data.write.data,
				   expt->current_test->addr_index, expt->current_test->data.write.at);
			break;
		default:
			break;
	}

	printk("\nWith %s %s memory checking %s %s memory\n", memory_type_names[expt->ctx.memory],
		   user_type_names[expt->ctx.user], memory_type_names[ctx.memory], user_type_names[ctx.user]);
}

static void show_expectation(t_ctx ctx, t_expectation* expt, int expected, int current_use) {
	if (ctx.memory == PHYSICAL && ctx.user == USER)
		return;
	if (expt->ctx.memory == PHYSICAL && expt->ctx.user == USER)
		return;
	if (ctx.memory == PHYSICAL && expt->ctx.memory == PHYSICAL)
		return;

	bool ok = false;
	if (expected == 0) {
		ok = (expected == current_use);
	} else if (expected < 0) {
		ok = (current_use <= expected);
	} else if (expected > 0) {
		ok = (current_use >= expected);
	}
	if (ctx.memory == VIRTUAL && ctx.user != expt->ctx.user) {
		ok = (current_use == 0);
		expected = 0;
	}
	// if (ok) return;

	for (int i = 0; i < 25; i++) {
		printk("\n");
	}
	printf("%d %d\n", expected, current_use);
	printk("%s: %s expected in %s %s memory: %s\n", ok ? "OK" : "NOK", expected ? "diff" : "no diff",
		   memory_type_names[ctx.memory], user_type_names[ctx.user],
		   (!ok && (expt->ctx.user != ctx.user)) ? "WARNING! mismatch user!!!" : "");

	if (current_use)
		print_full_historic_memory_report(ctx, &expt->memory_report);
	printk("\n");
	print_debug(expt->current_test->where);
	printk("\n-> ");
	print_test_context(ctx, expt);
	press_any();
}

/*
 * Example tests suite
 */

void set_simple_test(t_test_suite* tests) {
	SET_TEST(tests, 0, [PHYSICAL] = +1, [VIRTUAL] = +1).alloc(42);

	SET_TEST(tests, 0, [PHYSICAL] = 0, [VIRTUAL] = 0).write(0, "hello");

	SET_TEST(tests, 0, [PHYSICAL] = -1, [VIRTUAL] = -1).free();
}

void set_complex_test(t_test_suite* tests) {
	SET_TEST(tests, 0, [PHYSICAL] = +1, [VIRTUAL] = +1).alloc(42);
	SET_TEST(tests, 1, [PHYSICAL] = +1, [VIRTUAL] = +1).alloc(4295);

	SET_TEST(tests, 1, [PHYSICAL] = 0, [VIRTUAL] = 0).write(23, "hello");
	SET_TEST(tests, 1, [PHYSICAL] = +1, [VIRTUAL] = 0).write(4196, "hello2");

	SET_TEST(tests, 0, [PHYSICAL] = -1, [VIRTUAL] = -1).free();
	SET_TEST(tests, 1, [PHYSICAL] = -1, [VIRTUAL] = -1).free();
}
