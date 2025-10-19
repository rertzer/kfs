#include "fork.h"
#include "processus.h"
#include "scheduler.h"
#include "tss.h"

int16_t fork() {
	caller_data_t caller_data;
	__asm__ volatile(
		" \
	movl 4(%%ebp), %0; \
	movl (%%ebp), %1; \
	movl %%ebp, %2;"
		: "=g"(caller_data.ret_address), "=g"(caller_data.old_ebp), "=g"(caller_data.old_esp)::"memory");

	caller_data.old_esp -= 8;
	proc_t* parent = scheduler_get_current_proc();
	if (parent == NULL) {
		return (ERROR_FORK_NO_PARENT);
	}
	proc_t* child = spawn(parent, caller_data);
	if (child == NULL) {
		return (ERROR_FORK_SPAWN_FAILED);
	}
	size_t gdt_index = gdt_add_tss_descriptor(child->tss);
	if (gdt_index == 0) {
		free_process(child);
		return (ERROR_FORK_ADD_GDT_FAILED);
	}
	proc_set_gdt_index(child, gdt_index);
	scheduler_add_task(child);
	scheduler_run(child);
	return (child->pid);
}
