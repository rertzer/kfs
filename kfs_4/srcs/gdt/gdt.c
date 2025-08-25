#include "gdt.h"
#include "printk.h"

gdt_entry_t* gdt = (gdt_entry_t*)GDT_BUFFER;

static inline uint32_t get_gdt_limit();
static void			   add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc);

void init_gdt() {
	gdt_descriptor_t null_desc = (gdt_descriptor_t){0, 0, 0, 0};
	gdt_descriptor_t kernel_code_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_KERNEL_CODE_ACCESS, GDT_FLAGS};
	gdt_descriptor_t kernel_data_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_KERNEL_DATA_ACCESS, GDT_FLAGS};
	gdt_descriptor_t kernel_stack_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_KERNEL_STACK_ACCESS, GDT_FLAGS};
	gdt_descriptor_t user_code_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_USER_CODE_ACCESS, GDT_FLAGS};
	gdt_descriptor_t user_data_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_USER_DATA_ACCESS, GDT_FLAGS};
	gdt_descriptor_t user_stack_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_USER_STACK_ACCESS, GDT_FLAGS};
	gdt_descriptor_t bad_stack_desc = (gdt_descriptor_t){0, 0xFFFFF, GDT_BAD_STACK_ACCESS, GDT_FLAGS};

	add_gdt_descriptor(&gdt[0], null_desc);
	add_gdt_descriptor(&gdt[1], kernel_code_desc);
	add_gdt_descriptor(&gdt[2], kernel_data_desc);
	add_gdt_descriptor(&gdt[3], kernel_stack_desc);
	add_gdt_descriptor(&gdt[4], user_code_desc);
	add_gdt_descriptor(&gdt[5], user_data_desc);
	add_gdt_descriptor(&gdt[6], user_stack_desc);
	add_gdt_descriptor(&gdt[7], bad_stack_desc);

	set_gdt(get_gdt_limit(), GDT_BUFFER);
	printk("- Global Descriptor Table OK\n");
}

static inline uint32_t get_gdt_limit() {
	return (sizeof(gdt_entry_t) * GDT_ENTRIES_NB - 1);
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
	entry->bytes[6] = (uint8_t)(desc.flags << 4);
}
