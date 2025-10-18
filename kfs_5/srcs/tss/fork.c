#include "fork.h"
#include "keycode.h"
#include "processus.h"
#include "scheduler.h"
#include "tss.h"

int16_t fork() {
	fork_data_t fork_data;
	__asm__ volatile(
		" \
	movl 4(%%ebp), %0; \
	movl (%%ebp), %1; \
	movl %%ebp, %2;"
		: "=g"(fork_data.ret_address), "=g"(fork_data.old_ebp), "=g"(fork_data.old_esp)::"memory");

	fork_data.old_esp -= 8;
	proc_t* parent = scheduler_get_current_proc();
	if (parent == NULL) {
		return (-1);
	}
	proc_t* child = spawn(parent, fork_data);
	if (child == NULL) {
		return (-2);
	}
	tss_set_return_address(child->tss, (void*)fork_data.ret_address);
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
