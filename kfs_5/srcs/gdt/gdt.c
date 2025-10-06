#include "gdt.h"
#include "kernel.h"
#include "kfs_bitmap.h"
#include "printk.h"
#include "string.h"
#include "tss.h"

gdt_entry_t* gdt = (gdt_entry_t*)GDT_BUFFER;
tss_t		 tss_placeholder;
tss_t		 tss_zero;
size_t		 gdt_bitmap[GDT_BITMAP_SIZE_T_SIZE];

static void				gdt_set_to_zero();
static void				init_gdt_descriptors();
static inline uint32_t	get_gdt_limit();
static void				add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc);
static gdt_descriptor_t get_gdt_desc_by_entry(gdt_entry_t* entry);

void init_gdt() {
	gdt_set_to_zero();
	init_gdt_descriptors();
	set_gdt(get_gdt_limit(), GDT_BUFFER);
	set_tss(&tss_zero, kernel_zero);
	gdt_bitmap_init();
	// tss_t* test = get_tss_addr_by_gdt_offset(0x48);
	// print_tss(test);
	// uint16_t tr_offset = store_task_register();
	// printk("tss_zero %08x, test %08x, tr %08x\n", &tss_zero, test, tr_offset);
	// print_gdt_descriptor_by_offset(0x48);
}

static void init_gdt_descriptors() {
	add_gdt_descriptor(&gdt[NULL_DESCRIPTOR], (gdt_descriptor_t){0, 0, 0, 0});
	add_gdt_descriptor(&gdt[KERNEL_CODE_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_KERNEL_CODE_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[KERNEL_DATA_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_KERNEL_DATA_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[KERNEL_STACK_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_KERNEL_STACK_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[USER_CODE_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_USER_CODE_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[USER_DATA_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_USER_DATA_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[USER_STACK_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_USER_STACK_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[BAD_STACK_DESC], (gdt_descriptor_t){0, 0xFFFFF, GDT_BAD_STACK_ACCESS, GDT_FLAGS});
	add_gdt_descriptor(&gdt[TSS_PLACEHOLDER],
					   (gdt_descriptor_t){(uint32_t)&tss_placeholder, get_tss_limit(), TSS_AVAIL_ACCESS, TSS_FLAGS});
	add_gdt_descriptor(&gdt[TSS_ZERO],
					   (gdt_descriptor_t){(uint32_t)&tss_zero, get_tss_limit(), TSS_AVAIL_ACCESS, TSS_FLAGS});
}

static void gdt_set_to_zero() {
	memset(gdt, '\0', sizeof(gdt_entry_t) * GDT_MAX_ENTRIES);
}

static inline uint32_t get_gdt_limit() {
	return (sizeof(gdt_entry_t) * GDT_MAX_ENTRIES - 1);
}

static void add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc) {
	entry->bytes[2] = (uint8_t)(desc.base & 0xFF);
	entry->bytes[3] = (uint8_t)((desc.base >> 8) & 0xFF);
	entry->bytes[4] = (uint8_t)((desc.base >> 16) & 0xFF);
	entry->bytes[7] = (uint8_t)((desc.base >> 24) & 0xFF);
	entry->bytes[0] = (uint8_t)(desc.limit & 0xFF);
	entry->bytes[1] = (uint8_t)((desc.limit >> 8) & 0xFF);
	entry->bytes[6] = (uint8_t)((desc.limit >> 16) & 0xFF);
	entry->bytes[5] = (uint8_t)(desc.access);
	entry->bytes[6] |= (uint8_t)(desc.flags << 4);
}

size_t add_tss_descriptor(tss_t* tss) {
	size_t index = gdt_bitmap_get_new();
	if (index != GDT_MAX_ENTRIES) {
		add_gdt_descriptor(&gdt[index],
						   (gdt_descriptor_t){(uint32_t)tss, get_tss_limit(), TSS_AVAIL_ACCESS, TSS_FLAGS});
	} else {
		index = 0;
	}
	return (index);
}

void remove_tss_descriptor(size_t index) {
	if (index < GDT_MAX_ENTRIES) {
		gdt_bitmap_remove(index);
		add_gdt_descriptor(&gdt[index], (gdt_descriptor_t){0, 0, 0, 0});
	}
}

uint16_t get_gdt_init_desc_offset(init_gdt_descriptor_e d) {
	return (d * sizeof(gdt_entry_t));
}

gdt_descriptor_t get_gdt_init_desc(init_gdt_descriptor_e d) {
	return (get_gdt_desc_by_entry(&gdt[d]));
}

gdt_descriptor_t get_gdt_desc_by_offset(uint32_t offset) {
	uint32_t addr = (uint32_t)gdt + offset;
	return (get_gdt_desc_by_entry((gdt_entry_t*)addr));
}

static gdt_descriptor_t get_gdt_desc_by_entry(gdt_entry_t* entry) {
	gdt_descriptor_t desc = {0, 0, 0, 0};

	desc.base = entry->bytes[2] | entry->bytes[3] << 8 | entry->bytes[4] << 16 | entry->bytes[7] << 24;
	desc.limit = entry->bytes[0] | entry->bytes[1] << 8 | (entry->bytes[6] & 0xF) << 16;
	desc.access = entry->bytes[5];
	desc.flags = (entry->bytes[6] & 0xF0) >> 4;

	return (desc);
}

void print_gdt() {
	for (size_t index = 0; index < GDT_MAX_ENTRIES; ++index) {
		gdt_entry_t* entry = &gdt[index];
	}
}

void print_gdt_init_descriptor(init_gdt_descriptor_e d) {
	gdt_descriptor_t desc = get_gdt_init_desc(d);
	print_gdt_descriptor(desc);
}

void print_gdt_descriptor_by_offset(uint32_t offset) {
	gdt_descriptor_t desc = get_gdt_desc_by_offset(offset);
	print_gdt_descriptor(desc);
}

void print_gdt_descriptor(gdt_descriptor_t desc) {
	printk("base: %08x\tlimit:%08x\naccess:%08x\tflags:%08x\n", desc.base, desc.limit, desc.access, desc.flags);
}

void gdt_bitmap_init() {
	bitmap_erase(gdt_bitmap, GDT_MAX_ENTRIES);
	for (size_t i = 0; i < INIT_DESCRIPTOR_NB; ++i) {
		set_bitmap_value(gdt_bitmap, i, 1);
	}
}

size_t gdt_bitmap_get_new() {
	size_t new_index = get_first_bitmap(gdt_bitmap, GDT_MAX_ENTRIES);
	if (new_index != GDT_MAX_ENTRIES) {
		set_bitmap_value(gdt_bitmap, new_index, 1);
	}
	return (new_index);
}

void gdt_bitmap_remove(size_t index) {
	set_bitmap_value(gdt_bitmap, index, 0);
}

/* =============================== TEST =================================== */
void gdt_bitmap_test() {
	gdt_bitmap_init();

	printf("gdt bitmap testing\n");
	for (size_t i = 0; i < INIT_DESCRIPTOR_NB; ++i) {
		if (get_bitmap_value(gdt_bitmap, i) != 1) {
			printf("GDT_BITMAP_TEST ERROR 1\n");
			break;
		}
	}
	for (size_t i = INIT_DESCRIPTOR_NB; i < GDT_MAX_ENTRIES; ++i) {
		if (get_bitmap_value(gdt_bitmap, i) != 0) {
			printf("GDT_BITMAP_TEST ERROR 2\n");
			break;
		}
	}

	printf("testing get new \n");
	for (size_t i = INIT_DESCRIPTOR_NB; i < GDT_MAX_ENTRIES; ++i) {
		uint32_t index = gdt_bitmap_get_new();

		if (index != i) {
			printf("GDT_BITMAP_TEST ERROR new index: %d expected: %d\n", index, i);
			break;
		}
	}

	uint32_t index = gdt_bitmap_get_new();
	if (index != GDT_MAX_ENTRIES) {
		printf("GDT TEST ERROR: to many active entries %d \n", index);
	}
	printf("removing some...\n");
	gdt_bitmap_remove(42);
	index = gdt_bitmap_get_new();
	if (index != 42) {
		printf("pid bitmap error\n");
	}

	for (size_t i = 0; i < 3000; ++i) {
		size_t expected = 42 + (i % 666);
		gdt_bitmap_remove(expected);
		index = gdt_bitmap_get_new();
		if (index != expected) {
			printf("ERROR  %d %d\n", index, expected);
			break;
		}
	}
}
