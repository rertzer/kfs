#include "interrupts.h"
#include "builtin.h"
#include "panic.h"

extern volatile uint32_t						  hereafter;
extern uint32_t									  isr_stub_table[];
__attribute__((aligned(0x10))) static idt_entry_t idt[256];

static idtr_t idtr;

void init_idt() {
	idtr.base = (uint32_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (uint8_t vector = 0; vector < IDT_MAX_DESCRIPTORS; ++vector) {
		idt_set_descriptor(vector, isr_stub_table[vector], IDT_FLAG_PRESENT | IDT_FLAG_32BIT_INTERRUPT);
	}
	__asm__ volatile("lidt %0" : : "m"(idtr));
	__asm__ volatile("sti");
	printk("- Interrupt Descriptor table OK\n");
}

void idt_set_descriptor(uint8_t vector, uint32_t isr, uint8_t flags) {
	idt_entry_t* descriptor = &idt[vector];

	descriptor->isr_low = (uint32_t)isr & 0xFFFF;
	descriptor->kernel_cs = 0x08;
	descriptor->reserved = 0;
	descriptor->attributes = flags;
	descriptor->isr_high = (uint32_t)isr >> 16;
}

void general_protection_handler(uint32_t registers[8], uint32_t error_code) {
	(void)registers;
	printk("general protection fault\nerror code: %08x", error_code);
}

void exception_handler() {
	uint32_t ex_number = hereafter;
	hereafter = 666;
	printk("exception %d !\n", ex_number);
	registers(NULL, 0);
	__asm__ volatile("cli; hlt");
}

void default_exception_handler(uint32_t int_nb) {
	switch (int_nb) {
		case (0):
			panic("Divide by zero");
			break;
		case (1):
			printf("Debug Exception -- not yet handled\n");
			break;
		case (2):
			panic("Non Maskable Interrupt");
			break;
		case (3):
			printf("Breakpoint Interrupt -- not yet handled\n");
			break;
		case (4):
			panic("Overflow Exception");
			break;
		default:
			panic("unknown exception");
			break;
	}
}
