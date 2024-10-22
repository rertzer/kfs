#include "kernel.h"

__attribute__((aligned(0x10)))
gdt_ptr_t gdt_ptr;

__attribute__((aligned(0x10)))
gdt_entry_t* gdt = (gdt_entry_t*)GDT_BUFFER;

static void add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc);

void init_gdt() {
	gdt_ptr.limit = (uint16_t)sizeof(gdt_entry_t) * GDT_SIZE - 1;
	gdt_ptr.base = (uint32_t)GDT_BUFFER;

	gdt_descriptor_t gdt_descriptor_table[GDT_SIZE] = {GDT_TABLE};
	for (size_t i = 0; i < GDT_SIZE; ++i) {
		add_gdt_descriptor(&gdt[i], gdt_descriptor_table[i]);
	}

	gdt_flush((uint32_t)&gdt_ptr);
}

static void add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc) {
    entry->limit_low = (desc.limit & 0xFF);
    entry->base_low = (desc.base & 0xFF);
    entry->base_middle = (desc.base >> 16) & 0xFF;
    entry->access_byte = desc.access;
    entry->limit_high_flags = ((desc.limit >> 16) & 0x0F) | (desc.flags << 4 | 0x0F);
    entry->base_high = (desc.base >> 24) & 0xFF;
}
