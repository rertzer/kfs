#include "builtin.h"
#include "kernel.h"

static inline void init_regs(uint32_t* regs);
static void		   print_registers(uint32_t* regs);

uint8_t registers(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	uint32_t regs[REGS_NB];

	init_regs(regs);
	get_registers(regs);
	print_registers(regs);

	return (0);
}

static inline void init_regs(uint32_t* regs) {
	for (uint32_t i = 0; i < REGS_NB; ++i) {
		regs[i] = 0;
	}
}

static void print_registers(uint32_t* regs) {
	printk("eax: 0x%08x\tebx: 0x%08x\n", regs[0], regs[1]);
	printk("ecx: 0x%08x\tedx: 0x%08x\n", regs[2], regs[3]);
	printk("ebp: 0x%08x\tesp: 0x%08x\n", regs[4], regs[5]);
	printk("esi: 0x%08x\tedi: 0x%08x\n", regs[6], regs[7]);
	printk("cs : 0x%04x\t\tss : 0x%04x\n", regs[13], regs[8]);
	printk("ds : 0x%04x\t\tes : 0x%04x\n", regs[9], regs[10]);
	printk("fs : 0x%04x\t\tgs : 0x%04x\n", regs[11], regs[12]);
	printk("cr0: 0x%08x\tcr3: 0x%08x\n", regs[14], regs[15]);
	printk("flags : 0x%08x\n", regs[16]);
	printk("ret address: 0x%08x\n", get_retaddr());
}
