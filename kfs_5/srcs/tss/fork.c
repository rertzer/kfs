#include "fork.h"
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
	scheduler_add_task(child);
	return (child->pid);
}

uint8_t fork_return() {
	return (0);
}
