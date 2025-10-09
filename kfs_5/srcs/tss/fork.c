#include "fork.h"
#include "gdt.h"
#include "processus.h"
#include "scheduler.h"
#include "tss.h"

int16_t fork() {
	void* ret_address;
	__asm__ volatile("movl 4(%%ebp), %0" : "=r"(ret_address));
	proc_t* parent = scheduler_get_current_proc();
	if (parent == NULL) {
		return (-1);
	}
	proc_t* child = spawn(parent);
	if (child == NULL) {
		return (-2);
	}
	tss_set_return_address(child->tss, ret_address);
	size_t gdt_index = add_tss_descriptor(child->tss);
	if (gdt_index == 0) {
		free_process(child);
		return (-3);
	}
	proc_set_gdt_index(child, gdt_index);
	scheduler_add_task(child);
	scheduler_run(child);
	return (child->pid);
}
