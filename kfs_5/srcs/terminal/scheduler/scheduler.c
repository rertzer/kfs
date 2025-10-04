#include "scheduler.h"
#include "list_head.h"

list_head_t tasklist;
list_head_t runqueue;
proc_t*		current;

void scheduler_init(proc_t* proc_zero) {
	pid_bitmap_init();
	list_head_init(&tasklist);
	list_head_init(&runqueue);
	scheduler_add_task(proc_zero);
	scheduler_run(proc_zero);
	current = proc_zero;
}

uint8_t scheduler_add_task(proc_t* task) {
	increase_family(task);
	list_add(&tasklist, task);
	if (task->status == PROC_RUN) {
		list_add(&runqueue, task);
	}
	return (0);
}

uint8_t scheduler_run(proc_t* task) {
	if (task->status >= PROC_ZOMBIE) {
		return (1);
	}
	task->status = PROC_RUN;
	list_add(task, &task->run_lst);
	return (0);
}

uint8_t scheduler_remove_task(proc_t* task) {
	if (task->status != PROC_DEAD) {
		return (1);
	}
	decrease_family(task);
	list_extract(task);
	free_process(task);
	return (0);
}

void increase_family(proc_t* task) {}
void decrease_family(proc_t* task) {}
