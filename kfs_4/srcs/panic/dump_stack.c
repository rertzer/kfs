#include "panic.h"

volatile uint32_t* dump_addr = (uint32_t*)0xC0100000;
extern uint32_t	   stack_top;
extern uint32_t	   stack_bottom;

void dump_stack() {
	uint32_t stack_size = &stack_bottom - &stack_top;
	for (uint32_t i = 0; i <= stack_size; ++i) {
		dump_addr[i] = *(&stack_top + i);
	}
}

