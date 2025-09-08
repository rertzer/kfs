#include "builtin.h"
#include "kernel.h"

static inline void init_regs(uint32_t* regs);
static void		   print_registers(uint32_t* regs);

uint8_t registers(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
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
	printk("eax: %08x\tebx: %08x\n", regs[REG_EAX], regs[REG_EBX]);
	printk("ecx: %08x\tedx: %08x\n", regs[REG_ECX], regs[REG_EDX]);
	printk("ebp: %08x\tesp: %08x\n", regs[REG_EBP], regs[REG_ESP]);
	printk("esi: %08x\tedi: %08x\n", regs[REG_ESI], regs[REG_EDI]);
	printk("cs : %04x\t\tss : %04x\n", regs[REG_CS], regs[REG_SS]);
	printk("ds : %04x\t\tes : %04x\n", regs[REG_DS], regs[REG_ES]);
	printk("fs : %04x\t\tgs : %04x\n", regs[REG_FS], regs[REG_GS]);
	printk("cr0: %08x\tcr2: %08x\n", regs[REG_CR0], regs[REG_CR2]);
	printk("cr3: %08x\t", regs[REG_CR3]);
	printk("flags : %08x\n", regs[REG_EFLAGS]);
	printk("ret address: %08x\n", get_retaddr());
}
