#include "gdt.h"
#include "printk.h"
#include "tss.h"
#include "utils.h"

gdt_entry_t* gdt = (gdt_entry_t*)GDT_BUFFER;
tss_t		 tss_placeholder;
tss_t		 tss_zero;

static void				gdt_set_to_zero();
static void				init_gdt_descriptors();
static inline uint32_t	get_gdt_limit();
static void				add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc);
static gdt_descriptor_t get_gdt_desc_by_entry(gdt_entry_t* entry);

void init_gdt() {
	gdt_set_to_zero();
	init_gdt_descriptors();
	set_gdt(get_gdt_limit(), GDT_BUFFER);
	set_tss_default(&tss_zero);
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
					   (gdt_descriptor_t){(uint32_t)&tss_placeholder, get_tss_limit(), TSS_ABSENT_ACCESS, TSS_FLAGS});
	add_gdt_descriptor(&gdt[TSS_ZERO],
					   (gdt_descriptor_t){(uint32_t)&tss_zero, get_tss_limit(), TSS_AVAIL_ACCESS, TSS_FLAGS});
}

static void gdt_set_to_zero() {
	ft_memset(gdt, '\0', sizeof(gdt_entry_t) * GDT_MAX_ENTRIES);
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

gdt_descriptor_t get_gdt_desc(init_gdt_descriptor_e d) {
	return (get_gdt_desc_by_entry(&gdt[d]));
}

gdt_descriptor_t get_gdt_desc_by_offset(uint32_t offset) {
	uint32_t addr = (uint32_t)gdt + offset;
	return (get_gdt_desc_by_entry((gdt_entry_t*)addr));
}

static gdt_descriptor_t get_gdt_desc_by_entry(gdt_entry_t* entry) {
	gdt_descriptor_t desc = {0, 0, 0, 0};

	desc.base = entry->bytes[2] | entry->bytes[3] << 8 | entry->bytes[4] << 16 | entry->bytes[7];
	desc.limit = entry->bytes[0] | entry->bytes[1] << 8 | (entry->bytes[6] & 0xF) << 16;
	desc.access = entry->bytes[5];
	desc.flags = (entry->bytes[6] & 0xF0) >> 4;

	return (desc);
}

void print_gdt_descriptor(init_gdt_descriptor_e d) {
	gdt_descriptor_t desc = get_gdt_desc(d);
	printk("base: %08x\tlimit:%08x\naccess:%08x\tflags:%08x\n", desc.base, desc.limit, desc.access, desc.flags);
}
