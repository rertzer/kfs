#include "kernel.h"

extern uint32_t stack_top;
extern uint32_t stack_bottom;

void print_line(uint32_t *stack_pointer) {
	uint32_t *stack_pointer_end = stack_pointer + 6;
	printk("%08x	", stack_pointer);
	for (uint32_t *i = stack_pointer; i < stack_pointer_end; i++) {
		printk("%08x ", *i);
	}
	printk("\n");
}

uint8_t hexdump(void) {
	char testB[6000] = {};
	for (size_t i = 0; i < sizeof(testB); i++) {
		testB[i] = 'B';
	}
	for (uint32_t *stack_pointer = &stack_top; stack_pointer <= &stack_bottom; stack_pointer++) {		
		print_line(stack_pointer);
    }

	printk("Stack Bottom: 0x%08x\n", &stack_top);
	printk("Stack top: 0x%08x\n", &stack_bottom);

	printk("Stack size: %d\n", (uint32_t)&stack_bottom - (uint32_t)&stack_top);

	return (0);
}
