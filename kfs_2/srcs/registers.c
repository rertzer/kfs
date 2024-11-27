#include "builtin.h"
#include "kernel.h"

uint8_t registers() {
	uint32_t regs[REGS_NB];

	for (uint32_t i = 0; i < REGS_NB; ++i) {
		regs[i] = 0;
	}

	asm(" \
		mov %%eax, %0; \
		mov %%ebx, %1; \
		mov %%ecx, %2; \
		mov %%edx, %3; \
		mov %%ebp, %4; \
		mov %%esp, %5; \
		mov %%esi, %6; \
		mov %%edi, %7; \
		mov %%ss, %8; \
		mov %%ds, %9; \
		mov %%es, %10; \
		mov %%fs, %11; \
		mov %%gs, %12; \
		mov %%cs, %13"
		: "=m"(regs[0]), "=m"(regs[1]), "=m"(regs[2]), "=m"(regs[3]), "=m"(regs[4]), "=m"(regs[5]),
		  "=m"(regs[6]), "=m"(regs[7]), "=m"(regs[8]), "=m"(regs[9]), "=m"(regs[10]),
		  "=m"(regs[11]), "=m"(regs[12]), "=m"(regs[13])
		:
		: "memory");
	printk("eax: 0x%08x\tebx: 0x%08x\n", regs[0], regs[1]);
	printk("ecx: 0x%08x\tedx: 0x%08x\n", regs[2], regs[3]);
	printk("ebp: 0x%08x\tesp: 0x%08x\n", regs[4], regs[5]);
	printk("esi: 0x%08x\tedi: 0x%08x\n", regs[6], regs[7]);
	printk("cs : 0x%04x\t\tss : 0x%04x\n", regs[13], regs[8]);
	printk("ds : 0x%04x\t\tes : 0x%04x\n", regs[8], regs[10]);
	printk("fs : 0x%04x\t\tgs : 0x%04x\n", regs[11], regs[12]);
	return (0);
}
