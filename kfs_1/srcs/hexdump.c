#include "kernel.h"

static uint32_t* get_stack_pointer(void) {
	uint32_t* ebp;
	__asm__ volatile("movl %%ebp, %0" : "=r"(ebp));
	return (ebp);
}

uint8_t hexdump(void) {
	int			frame = 0;
	uint32_t	*ebp = get_stack_pointer();
	
	for (; ebp[0] && frame < 10; frame++) {
        printk("  #%d  %08x\n", frame, ebp[1]);
        ebp = (uint32_t *)(ebp[0]);
    }
	printk("  #%d  %08x\n", frame, ebp[1]);
	return (0);
}
