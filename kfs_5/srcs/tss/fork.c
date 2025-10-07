#include "fork.h"
#include "gdt.h"
#include "processus.h"
#include "scheduler.h"

int16_t fork() {
	proc_t* parent = scheduler_get_current_proc();
	if (parent == NULL) {
		return (-1);
	}
	proc_t* child = spawn(parent);
	if (child == NULL) {
		return (-2);
	}
	size_t gdt_index = add_tss_descriptor(child->tss);
	if (gdt_index == 0) {
		free_process(child);
		return (-3);
	}
	proc_set_gdt_index(child, gdt_index);
	scheduler_add_task(child);
	return (child->pid);
}

uint8_t fork_return() {
	return (0);
}
