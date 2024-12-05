#include "interrupts.h"
#include "kernel.h"

extern volatile uint32_t						  hereafter;
extern uint32_t									  isr_stub_table[];
__attribute__((aligned(0x10))) static idt_entry_t idt[256];

static idtr_t idtr;
static bool	  vectors[IDT_MAX_DESCRIPTORS];

void init_idt() {
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
	uint32_t ex_number = hereafter;
	hereafter = 666;
	printk("exception %d !\n", ex_number);
	// __asm__ volatile("cli; hlt");
}

#define INTERRUPT \
	[0] = "int $0x00", [1] = "int $0x01", [2] = "int $0x02", [3] = "int $0x03", \
	[4] = "int $0x04", [5] = "int $0x05", [6] = "int $0x06", [7] = "int $0x07", \
	[8] = "int $0x08", [9] = "int $0x09", [10] = "int $0x0A", [11] = "int $0x0B", \
	[12] = "int $0x0C", [13] = "int $0x0D", [14] = "int $0x0E", [15] = "int $0x0F", \
	[16] = "int $0x10", [17] = "int $0x11", [18] = "int $0x12", [19] = "int $0x13", \
	[20] = "int $0x14", [21] = "int $0x15", [22] = "int $0x16", [23] = "int $0x17", \
	[24] = "int $0x18", [25] = "int $0x19", [26] = "int $0x1A", [27] = "int $0x1B", \
	[28] = "int $0x1C", [29] = "int $0x1D", [30] = "int $0x1E", [31] = "int $0x1F", 


uint8_t trigger_interrupt(char *word, size_t word_len) {
	if (word_len == 0) {
		printk("usage: int <interrupt_number>\n");
		return (1);
	}

	int interrupt_number = ft_atoi(word);

	switch (interrupt_number) {
		case 0x00:
			asm volatile("int $0x00");
			break;
		case 0x01:
			asm volatile("int $0x01");
			break;
		case 0x02:
			asm volatile("int $0x02");
			break;
		case 0x03:
			asm volatile("int $0x03");
			break;
		case 0x04:
			asm volatile("int $0x04");
			break;
		case 0x05:
			asm volatile("int $0x05");
			break;
		case 0x06:
			asm volatile("int $0x06");
			break;
		case 0x07:
			asm volatile("int $0x07");
			break;
		case 0x08:
			asm volatile("int $0x08");
			break;
		case 0x09:
			asm volatile("int $0x09");
			break;
		case 0x0A:
			asm volatile("int $0x0A");
			break;
		case 0x0B:
			asm volatile("int $0x0B");
			break;
		case 0x0C:
			asm volatile("int $0x0C");
			break;
		case 0x0D:
			asm volatile("int $0x0D");
			break;
		case 0x0E:
			asm volatile("int $0x0E");
			break;
		case 0x0F:
			asm volatile("int $0x0F");
			break;
		case 0x10:
			asm volatile("int $0x10");
			break;
		case 0x11:
			asm volatile("int $0x11");
			break;
		case 0x12:
			asm volatile("int $0x12");
			break;
		case 0x13:
			asm volatile("int $0x13");
			break;
		case 0x14:
			asm volatile("int $0x14");
			break;
		case 0x15:
			asm volatile("int $0x15");
			break;
		case 0x16:
			asm volatile("int $0x16");
			break;
		case 0x17:
			asm volatile("int $0x17");
			break;
		case 0x18:
			asm volatile("int $0x18");
			break;
		case 0x19:
			asm volatile("int $0x19");
			break;
		case 0x1A:
			asm volatile("int $0x1A");
			break;
		case 0x1B:
			asm volatile("int $0x1B");
			break;
		case 0x1C:
			asm volatile("int $0x1C");
			break;
		case 0x1D:
			asm volatile("int $0x1D");
			break;
		case 0x1E:
			asm volatile("int $0x1E");
			break;
		case 0x1F:
			asm volatile("int $0x1F");
			break;
		default:
			break;
	}

	return (0);
}
