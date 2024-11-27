#include "builtin.h"
#include "kernel.h"

uint8_t registers() {
	uint32_t regs[17];
	// uint32_t test = 0x42;

	asm(" \
		mov %%eax, %0; \
		mov %%ebx, %1; \
		mov %%ecx, %2; \
		mov %%edx, %3; \
		mov %%ebp, %4; \
		mov %%esp, %5; \
		mov %%esi, %6; \
		mov %%edi, %7"
		: "=m"(regs[0]), "=m"(regs[1]), "=m"(regs[2]), "=m"(regs[3]), "=m"(regs[4]), "=m"(regs[5]),
		  "=m"(regs[6]), "=m"(regs[7])
		:
		: "memory");
	printk("eax: %x\tebx: %x\n", regs[0], regs[1]);
	printk("ecx: %x\tedx: %x\n", regs[2], regs[3]);
	printk("ebp: %x\tesp: %x\n", regs[4], regs[5]);
	printk("esi: %x\tedi: %x\n", regs[6], regs[7]);
	return (0);
}
