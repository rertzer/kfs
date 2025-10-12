#include "scheduler.h"
#include "list_head.h"
#include "printk.h"
#include "processus.h"

list_head_t tasklist;
list_head_t runqueue;
proc_t*		current;

void scheduler() {
	printf("current pid %d\n", current->pid);
	current = list_round(&runqueue, PROC_LIST_RUNQUEUE);
	printf("current pid %d\n", current->pid);
	printf("current tss address %p\n", current);
	printf("new stack %x %x %d\n", current->tss->esp, current->kernel_stack, ((uint32_t*)current->tss->esp)[0]);
	printf("new stack %x\n", current->tss->esp);
	switch_task(current->gdt_index);
}

void scheduler_init(proc_t* proc_zero) {
	pid_bitmap_init();
	list_head_init(&tasklist);
	list_head_init(&runqueue);
	scheduler_add_task(proc_zero);
	// scheduler_run(proc_zero);
	current = proc_zero;
}

uint8_t scheduler_add_task(proc_t* task) {
	increase_family(task);
	list_add(task, &tasklist);
	if (task->status == PROC_RUN) {
		list_add(&task->run_lst, &runqueue);
	}
	return (0);
}

uint8_t scheduler_run(proc_t* task) {
	if (task->status >= PROC_ZOMBIE) {
		return (1);
	}
	task->status = PROC_RUN;
	list_add(&task->run_lst, &runqueue);
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

proc_t* scheduler_get_current_proc() {
	return (current);
}

list_head_t* scheduler_get_tasklist() {
	return (&tasklist);
}

void increase_family(proc_t* task) {}
void decrease_family(proc_t* task) {}

uint16_t getpid() {
	return (current->pid);
}
