#include "tss.h"
#include "builtin.h"
#include "gdt.h"
#include "keycode.h"
#include "malloc.h"
#include "memory.h"
#include "panic.h"
#include "printk.h"
#include "scheduler.h"
#include "string.h"

uint32_t get_tss_limit() {
	return (sizeof(tss_t) - 1);
}

void run_task_zero() {
	uint16_t placeholder_offset = get_gdt_init_desc_offset(TSS_PLACEHOLDER);
	uint16_t zero_offset = get_gdt_init_desc_offset(TSS_ZERO);
	load_task_register(placeholder_offset);
	proc_t* zero_proc = init_zero_proc();
	scheduler_init(zero_proc);
	task_switch(zero_offset);
	panic("zero task switch failed");
}

void switch_task(uint32_t index) {
	uint16_t offset = index * sizeof(gdt_entry_t);
	task_switch(offset);
}

void set_tss(tss_t* tss, void* fun) {
	memset(tss, '\0', sizeof(tss_t));
	init_tss_registers(tss, fun);
}

tss_t* get_tss_addr_by_gdt_offset(uint32_t offset) {
	gdt_descriptor_t desc = get_gdt_desc_by_offset(offset);
	return ((tss_t*)desc.base);
}

tss_t* spawn_tss(caller_data_t caller_data) {
	tss_t* tss = kmalloc(sizeof(tss_t));
	if (tss == NULL) {
		return (NULL);
	}
	memset(tss, '\0', sizeof(tss_t));

	fork_registers_to_tss(tss);

	tss->ebp = caller_data.old_ebp;
	tss->esp0 = caller_data.old_esp;
	tss->esp1 = caller_data.old_esp;
	tss->esp2 = caller_data.old_esp;
	tss->esp = caller_data.old_esp;
	tss->eip = caller_data.ret_address;

	return (tss);
}

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
		tss->backlink, tss->esp0, tss->ss0, tss->esp1, tss->ss1, tss->esp2, tss->ss2, tss->cr3, tss->eip, tss->eflags,
		tss->eax, tss->ecx, tss->edx, tss->ebx, tss->esp, tss->ebp, tss->esi, tss->edi, tss->es, tss->cs, tss->ss,
		tss->ds, tss->fs, tss->gs, tss->ldt, tss->trace, tss->iomap);
}
