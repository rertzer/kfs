#include "processus.h"
#include "malloc.h"
#include "panic.h"
#include "utils.h"

uint16_t pid_tss_table[MAX_PROC_NB];
proc_t*	 proc_table[MAX_PROC_NB];

void init_porcessus() {
	ft_memset(&pid_tss_table, '\0', sizeof(pid_tss_table));
	ft_memset(&proc_table, '\0', sizeof(proc_table));
}

proc_t init_zero_proc() {
	proc_t proc;
	proc.pid = 0;
	proc.owner = 0;
	proc.father = NULL;
	proc.childrens = NULL;
	proc.tss = get_tss_addr_by_gdt_offset(TSS_ZERO);
	proc.signals = NULL;
	proc.status = SLEEP;
	proc.stack = (uint8_t*)proc.tss->esp;
	proc.heap = NULL;

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
