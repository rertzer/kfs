#include "tss.h"
#include "printk.h"
#include "utils.h"

uint32_t get_tss_limit() {
	return (sizeof(tss_t) - 1);
}

void set_tss_default(tss_t* tss) {
	ft_memset(tss, '\0', sizeof(tss_t));
	printk("before: \n");
	print_tss(tss);
	copy_registers_to_tss(tss);
	printk("after: \n");
	print_tss(tss);
}

void print_tss(tss_t* tss) {
	printk(
		" backlink : %p\n \
esp0: %p\n \
ss0: %p\n \
esp1: %p\n \
ss1: %p\n \
esp2: %p\n \
ss2: %p\n \
cr3: %p\n \
eip: %p\n \
eflags: %p\n \
eax: %p\n \
ecx:\t%p\n \
edx:\t%p\n \
ebx:\t%p\n \
esp:\t%p\n \
ebp:\t%p\n \
esi:\t%p\n \
edi:\t%p\n \
es:\t%p\n \
cs:\t%p\n \
ss:\t%p\n \
ds:\t%p\n \
fs:\t%p\n \
gs:\t%p\n \
ldt:\t%p\n \
trace:\t%b\n \
iomap:\t%d\n \
		",
		tss->backlink, tss->esp, tss->ss0, tss->esp1, tss->ss1, tss->esp2, tss->ss2, tss->cr3, tss->eip, tss->eflags,
		tss->eax, tss->ecx, tss->edx, tss->ebx, tss->esp, tss->ebp, tss->esi, tss->edi, tss->es, tss->cs, tss->ss,
		tss->ds, tss->fs, tss->gs, tss->ldt, tss->trace, tss->iomap);
}
