#include "processus.h"
#include "gdt.h"
#include "malloc.h"
#include "panic.h"
#include "string.h"

uint16_t pid_tss_table[MAX_PROC_NB];
proc_t*	 proc_table[MAX_PROC_NB];

void init_porcessus() {
	memset(&pid_tss_table, '\0', sizeof(pid_tss_table));
	memset(&proc_table, '\0', sizeof(proc_table));
}

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

void append_proc_table(proc_t p) {
	if (p.pid >= MAX_PROC_NB) {
		panic("Invalid pid");
	}
	proc_t* proc = kpmalloc(sizeof(proc_t));
	if (proc == NULL) {
		panic("kernel memory allocation error");
	}
	proc_table[p.pid] = proc;
}

void delete_proc_table(uint32_t pid) {
	if (pid >= MAX_PROC_NB || pid == 0) {
		panic("Invalid pid");
	}
	kpfree(proc_table[pid]);
	proc_table[pid] = 0;
}
