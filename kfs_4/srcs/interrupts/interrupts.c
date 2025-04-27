#include "interrupts.h"
#include "builtin.h"
#include "panic.h"
#include "stdlib.h"

extern volatile uint32_t						  hereafter;
extern uint32_t									  isr_stub_table[];
__attribute__((aligned(0x10))) static idt_entry_t idt[256];

static idtr_t idtr;

// static void empty_descriptor(uint8_t vector, uint32_t isr, uint8_t flags);
static void error_panic(char* msg, uint32_t error_code);

void init_idt() {
	idtr.base = (uint32_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (uint8_t vector = 0; vector < IDT_MAX_DESCRIPTORS; ++vector) {
#if 0
		// only to test segment not present
		if (vector == 21) {
			idt_set_descriptor(vector, isr_stub_table[vector], IDT_FLAG_32BIT_INTERRUPT);
			// } else if (vector == 13) {
			// 	empty_descriptor(vector, isr_stub_table[vector], IDT_FLAG_PRESENT | IDT_FLAG_32BIT_INTERRUPT);
		} else {
			idt_set_descriptor(vector, isr_stub_table[vector], IDT_FLAG_PRESENT | IDT_FLAG_32BIT_INTERRUPT);
		}
#else
		idt_set_descriptor(vector, isr_stub_table[vector], IDT_FLAG_PRESENT | IDT_FLAG_32BIT_INTERRUPT);
#endif
	}
	__asm__ volatile("lidt %0" : : "m"(idtr));
	__asm__ volatile("sti");
	printk("- Interrupt Descriptor table OK\n");
}
//
// static void empty_descriptor(uint8_t vector, uint32_t isr, uint8_t flags) {
// 	idt_entry_t* descriptor = &idt[vector];
//
// 	descriptor->isr_low = 0xdead;  //(uint32_t)isr & 0xFFFF;
// 	descriptor->kernel_cs = 0x18;
// 	descriptor->reserved = 0;
// 	descriptor->attributes = flags;
// 	descriptor->isr_high = (uint32_t)isr >> 16;
// }

void idt_set_descriptor(uint8_t vector, uint32_t isr, uint8_t flags) {
	idt_entry_t* descriptor = &idt[vector];

	descriptor->isr_low = (uint32_t)isr & 0xFFFF;
	descriptor->kernel_cs = 0x08;
	descriptor->reserved = 0;
	descriptor->attributes = flags;
	descriptor->isr_high = (uint32_t)isr >> 16;
}

void interrupt_handler(uint32_t int_nb) {
	printk("interrupt request %d !\n", int_nb);
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
		case (5):
			panic("Bound Exception");
			break;
		case (6):
			panic("Invalid Opcode");
			break;
		case (7):
			panic("Coprocessor Not Available");
			break;
		case (8):
			panic("double fault\n");
			break;
		case (9):
			panic("Coporocessor Segment Overrun");
			break;
		case (16):
			panic("Coprocessor Error");
			break;
		default:
			panic("unknown exception");
			break;
	}
}
void error_exception_handler(uint32_t int_nb, uint32_t error_code) {
	switch (int_nb) {
		case (10):
			error_panic("Invalid TSS Exception,    error code: 00000000", error_code);
			break;
		case (11):
			error_panic("Segment Not Present,      error code: 00000000", error_code);
			break;
		case (12):
			error_panic("Stack Exception,          error code: 00000000", error_code);
			break;
		case (13):
			error_panic("General Protection Fault, error code: 00000000", error_code);
			break;
		default:
			error_panic("Unknown Exception,        error code: 00000000", error_code);
			break;
	}
}

static void error_panic(char* msg, uint32_t error_code) {
	itoa(error_code, msg + 38, 16);
	panic(msg);
}
