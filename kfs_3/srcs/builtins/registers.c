#include "builtin.h"
#include "kernel.h"

uint8_t registers(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
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
		mov %%cs, %13; \
		mov %%cr0, %%eax; \
		mov %%eax, %14; \
		mov %%cr3, %%eax; \
		mov %%eax, %15; \
		pushf; \
		pop %16;"
		: "=m"(regs[0]), "=m"(regs[1]), "=m"(regs[2]), "=m"(regs[3]), "=m"(regs[4]), "=m"(regs[5]),
		  "=m"(regs[6]), "=m"(regs[7]), "=m"(regs[8]), "=m"(regs[9]), "=m"(regs[10]),
		  "=m"(regs[11]), "=m"(regs[12]), "=m"(regs[13]), "=m"(regs[14]), "=m"(regs[15]),
		  "=a"(regs[16])
		:
		: "memory");
	printk("eax: 0x%08x\tebx: 0x%08x\n", regs[0], regs[1]);
	printk("ecx: 0x%08x\tedx: 0x%08x\n", regs[2], regs[3]);
	printk("ebp: 0x%08x\tesp: 0x%08x\n", regs[4], regs[5]);
	printk("esi: 0x%08x\tedi: 0x%08x\n", regs[6], regs[7]);
	printk("cs : 0x%04x\t\tss : 0x%04x\n", regs[13], regs[8]);
	printk("ds : 0x%04x\t\tes : 0x%04x\n", regs[9], regs[10]);
	printk("fs : 0x%04x\t\tgs : 0x%04x\n", regs[11], regs[12]);
	printk("cr0: 0x%08x\t\tcr3: 0x%08x\n", regs[14], regs[15]);
	printk("flags : 0x%08x\n", regs[16]);
	printk("ret address: 0x%08x\n", get_retaddr());
	return (0);
}
