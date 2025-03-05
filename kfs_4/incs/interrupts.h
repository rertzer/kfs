#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 48
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_32BIT_INTERRUPT 0x0E
#define PAGE_TABLE_MASK 0x3FF000

typedef struct {
	uint16_t isr_low;	  // Lower 16 bits of the ISR's address
	uint16_t kernel_cs;	  // GDT segment selector loaded into CS
	uint8_t	 reserved;	  // Set to zero
	uint8_t	 attributes;  // Flags
	uint16_t isr_high;	  // Higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr_t;

void exception_handler();
void default_exception_handler(uint32_t int_nb);
void general_protection_handler(uint32_t registers[8], uint32_t error_code);
void page_fault_handler(uint32_t l_address, uint32_t error_code);
void keyboard_handler(uint8_t scan);
void idt_set_descriptor(uint8_t vector, uint32_t isr, uint8_t flags);
void init_idt(void);

// tests
void interrupts_test();
void zero_divide_test();
void debug_exception_test();
void nmi_test();
void breakpoint_test();
void overflow_test(uint32_t over);

#endif
