#include "processus.h"
#include "gdt.h"
#include "malloc.h"
#include "panic.h"
#include "string.h"

proc_t init_zero_proc() {
	proc_t proc;
	proc.pid = 0;
	proc.owner = 0;
	proc.father = NULL;
	proc.tss = get_tss_addr_by_gdt_offset(TSS_ZERO * sizeof(gdt_entry_t));
	proc.signals = NULL;
	proc.status = PROC_SLEEP;
	proc.stack = (uint8_t*)proc.tss->esp;
	proc.heap = NULL;
	list_head_init(&proc.lst);
	list_head_init(&proc.run_lst);
	list_head_init(&proc.childrens);
	list_head_init(&proc.siblings);

	return (proc);
}

uint8_t spawn(proc_t* src, proc_t* dest) {
	dest->pid = pid_bitmap_get_new();
	if (dest->pid == 0) {
		return (1);
	}
	dest->owner = src->owner;
	dest->father = src;
	// create tss
	dest->signals = NULL;
	dest->status = PROC_SLEEP;
	dest->stack = (uint8_t*)dest->tss->esp;
	// create heap
	// parenthood handled by the scheduler
	list_head_init(&dest->lst);
	list_head_init(&dest->run_lst);
	list_head_init(&dest->childrens);
	list_head_init(&dest->siblings);
	return (0);
}

void free_process(proc_t* task) {
	// free pid
	// remove from family
	// free tss
	// free stack
	// free memory
}
