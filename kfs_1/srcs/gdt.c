#include "kernel.h"

gdt_entry_t *gdt;
gdt_ptr_t gdt_ptr;

static void add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc);

void init_gdt() {
	gdt_descriptor_t gdt_descriptor_table[GDT_SIZE] = {GDT_TABLE};
	gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_SIZE);
	gdt_ptr.base = (uint32_t)gdt;

	for (size_t i = 0; i < GDT_SIZE; ++i) {
		add_gdt_descriptor(&gdt[i], gdt_descriptor_table[i]);
	}

	set_gdt(0, 0);
}

static void add_gdt_descriptor(gdt_entry_t* entry, gdt_descriptor_t desc) {
    entry[0] = desc.limit & 0xFF;
    entry[1] = (desc.limit >> 8) & 0xFF;
    entry[6] = (desc.limit >> 16) & 0x0F;
    
    entry[2] = desc.base & 0xFF;
    entry[3] = (desc.base >> 8) & 0xFF;
    entry[4] = (desc.base >> 16) & 0xFF;
    entry[7] = (desc.base >> 24) & 0xFF;
    
    entry[5] = desc.access;
    
    entry[6] |= (desc.flags << 4);
}
