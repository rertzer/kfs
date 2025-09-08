#include "tss.h"
#include "gdt.h"
#include "printk.h"
#include "utils.h"

uint32_t get_tss_limit() {
	return (sizeof(tss_t) - 1);
}

void create_tss_zero() {}

void set_tss_default(tss_t* tss) {
	ft_memset(tss, '\0', sizeof(tss_t));
	printk("before: \n");
	print_tss(tss);
	// copy_registers_to_tss(tss);
	init_tss_registers(tss);
	printk("after: \n");
	print_tss(tss);
	print_gdt_descriptor(USER_STACK_DESC);
}

tss_t* get_tss_addr_by_gdt_offset(uint32_t offset) {}

void print_tss(tss_t* tss) {
	printk(
		" backlink : %p\n \
esp0: %08x\tss0: %04x\n \
esp1: %08x\tss1: %04x\n \
esp2: %08x\tss2: %04x\n \
cr3: %08x\n \
eip: %08x\n \
eflags: %08x\n \
eax: %08x\tecx:\t%08x\n \
edx:\t%08x\tebx:\t%08x\n \
esp:\t%08x\tebp:\t%08x\n \
esi:\t%08x\tedi:\t%08x\n \
es:\t%04x\tcs:\t%04x\n \
ss:\t%04x\tds:\t%04x\n \
fs:\t%04x\tgs:\t%04x\n \
ldt:\t%p\n \
trace:\t%d\tiomap:\t%d\n ",
		tss->backlink, tss->esp, tss->ss0, tss->esp1, tss->ss1, tss->esp2, tss->ss2, tss->cr3, tss->eip, tss->eflags,
		tss->eax, tss->ecx, tss->edx, tss->ebx, tss->esp, tss->ebp, tss->esi, tss->edi, tss->es, tss->cs, tss->ss,
		tss->ds, tss->fs, tss->gs, tss->ldt, tss->trace, tss->iomap);
}
