#include "kernel.h"

void hexdump(void) {
	uint32_t	*ebp;
	int			frame;
	
	__asm__ volatile ("movl %%ebp, %0" : "=r" (ebp) ::);

	for (frame = 0; ebp[0] && frame < 10; frame++) {
        printk("  #%d  %08x\n", frame, ebp[1]);
        ebp = (uint32_t *)(ebp[0]);
    }
	printk("  #%d  %08x\n", frame, ebp[1]);
}
