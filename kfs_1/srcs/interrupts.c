#include "interrupts.h"
#include "kernel.h"

extern volatile uint32_t						  hereafter;
extern uint32_t									  isr_stub_table[];
__attribute__((aligned(0x10))) static idt_entry_t idt[256];

static idtr_t idtr;
static bool	  vectors[IDT_MAX_DESCRIPTORS];
void		  init_idt() {
	 idtr.base = (uint32_t)&idt[0];
	 idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	 for (uint8_t vector = 0; vector < IDT_MAX_DESCRIPTORS; ++vector) {
		 idt_set_descriptor(vector, isr_stub_table[vector],
									IDT_FLAG_PRESENT | IDT_FLAG_32BIT_INTERRUPT);
		 vectors[vector] = true;
	 }

	 vectors[7] = false;
	 __asm__ volatile("lidt %0" : : "m"(idtr));
	 __asm__ volatile("sti");
}

void idt_set_descriptor(uint8_t vector, uint32_t isr, uint8_t flags) {
	idt_entry_t* descriptor = &idt[vector];

	descriptor->isr_low = (uint32_t)isr & 0xFFFF;
	descriptor->kernel_cs = 0x08;
	descriptor->reserved = 0;
	descriptor->attributes = flags;
	descriptor->isr_high = (uint32_t)isr >> 16;
}

void exception_handler() {
	printk("exception %d !\n", hereafter);
	__asm__ volatile("cli; hlt");
}
