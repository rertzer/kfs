#include "kernel.h"

extern volatile uint32_t hereafter;
extern uint32_t isr_stub_table[];

__attribute__((aligned(0x10)))
static idt_ptr_t idt_ptr;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

static void add_idt_descriptor(idt_entry_t* entry, idt_descriptor_t desc);

void init_idt() {
	idt_ptr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
	idt_ptr.base = (uint32_t)&idt[0];

	for (uint8_t vector = 0; vector < IDT_MAX_DESCRIPTORS; ++vector) {
		idt_descriptor_t desc = {
			isr_stub_table[vector], 
			IDT_FLAG_PRESENT | IDT_FLAG_32BIT_INTERRUPT
		};
        add_idt_descriptor(&idt[vector], desc);
	}

	idt_flush((uint32_t)&idt_ptr);
}

static void add_idt_descriptor(idt_entry_t* entry, idt_descriptor_t desc) {
	entry->isr_low = ((uint32_t)desc.isr & 0xFFFF);
	entry->kernel_cs = 0x08;
	entry->reserved = 0;
	entry->attributes = desc.flags;
	entry->isr_high = ((uint32_t)desc.isr >> 16);
}

void exception_handler(void) {
	printk("Exception handler %d\n", hereafter);
    asm volatile ("cli; hlt");
}