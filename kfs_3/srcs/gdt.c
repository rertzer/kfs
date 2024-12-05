#include "kernel.h"

gdt_entry_t* gdt = (gdt_entry_t*)GDT_BUFFER;

static void add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc);

void init_gdt() {
	//                                                     base  limit  access flags
	gdt_descriptor_t null_desc = (gdt_descriptor_t){0, 0, 0, 0};
	gdt_descriptor_t kernel_code_desc = (gdt_descriptor_t){0, 0xFFFFF, 0x9A, 0xC};
	gdt_descriptor_t kernel_data_desc = (gdt_descriptor_t){0, 0xFFFFF, 0x92, 0xC};
	gdt_descriptor_t kernel_stack_desc = (gdt_descriptor_t){0, 0xFFFFF, 0x96, 0xC};
	gdt_descriptor_t user_code_desc = (gdt_descriptor_t){0, 0xFFFFF, 0xFA, 0xC};
	gdt_descriptor_t user_data_desc = (gdt_descriptor_t){0, 0xFFFFF, 0xF2, 0xC};
	gdt_descriptor_t user_stack_desc = (gdt_descriptor_t){0, 0xFFFFF, 0xF6, 0xC};

	add_gdt_descriptor(&gdt[0], null_desc);
	add_gdt_descriptor(&gdt[1], kernel_code_desc);
	add_gdt_descriptor(&gdt[2], kernel_data_desc);
	add_gdt_descriptor(&gdt[3], kernel_stack_desc);
	add_gdt_descriptor(&gdt[4], user_code_desc);
	add_gdt_descriptor(&gdt[5], user_data_desc);
	add_gdt_descriptor(&gdt[6], user_stack_desc);

	printk("before set_gdt\n");

	// printk("gdt: 0x%08x\n", gdt);
	// printk("GDT_BUFFER: 0x%08x\n", GDT_BUFFER);
	// uint32_t index = 0;
	//
	// for (uint32_t* i = (uint32_t*)gdt; i < (uint32_t*)gdt + 14; ++i) {
	// 	printk("gdt %08x: %08x \n", index, *i);
	// 	++index;
	// }
	set_gdt(sizeof(uint64_t) * 7 - 1, GDT_BUFFER);
	printk(
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\nFFFFFFFFFFFFFFFFF"
		"FFFFFFFFF\nTEREREREREREafter set_gdt\n");
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