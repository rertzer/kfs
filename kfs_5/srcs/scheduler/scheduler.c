#include "scheduler.h"
#include "kernel.h"
#include "keycode.h"
#include "malloc.h"
#include "printk.h"
#include "processus.h"
#include "signal.h"

#include "kfs_list_head.h"

list_head_t tasklist;
list_head_t runqueue;
proc_t*		current;
bool		scheduler_atomic;

void scheduler_enter() {
	// TODO a real atomic instruction
	while (scheduler_atomic == true) {
		halting();
	}
	scheduler_atomic = true;
}

void scheduler_leave() {
	scheduler_atomic = false;
}

bool scheduler_get_atomic() {
	return (scheduler_atomic);
}

int sched_yield(void) {
	scheduler();
	return (0);
}

void scheduler() {
	scheduler_enter();
	proc_t* previous = current;
	current = list_round(&runqueue, PROC_LIST_RUNQUEUE);

	scheduler_switch_task(current != previous);
}

void scheduler_switch_task(bool switching) {
	if (switching == true) {
		printk("switching task to %d\n", current->pid);
		switch_task(current->gdt_index);
	}
	printk("%d pending...%d\n", current->pid, current->sig_pending);
	pending_signals(current);

	if (current->status != PROC_RUN) {
		scheduler_enter();
		scheduler_switch_status();
		scheduler_switch_task(true);
	}
}

void scheduler_init(proc_t* proc_zero) {
	scheduler_atomic = false;
	pid_bitmap_init();
	list_head_init(&tasklist);
	list_head_init(&runqueue);
	scheduler_add_task(proc_zero);
	current = proc_zero;
}

uint8_t scheduler_add_task(proc_t* task) {
	family_growing(task);
	list_add(task, &tasklist);
	if (task->status == PROC_RUN) {
		list_add(&task->run_lst, &runqueue);
	}
	return (0);
}

uint8_t scheduler_set_current_status(proc_status_e status) {
	current->status = status;
	return (0);
}

int scheduler_get_current_exit_status() {
	return (current->exit_status);
}

uint8_t scheduler_set_current_exit_status(int sig) {
	current->exit_status = sig;
	return (0);
}

void scheduler_switch_status() {
	proc_t* previous = scheduler_unrun(current);
	switch (previous->status) {
		case PROC_RUN:
			scheduler_run(previous);
			break;
		case PROC_SLEEP:
			scheduler_sleep(previous);
			break;
		case PROC_STOPPED:
			scheduler_stopped(previous);
			break;
		case PROC_ZOMBIE:
			scheduler_zombie(previous);
			break;
		case PROC_DEAD:
			scheduler_dead(previous);
			break;
	}
}

proc_t* scheduler_unrun(proc_t* task) {
	proc_t* previous = list_extract_offset(&task->run_lst, PROC_LIST_RUNQUEUE);
	current = list_get(runqueue.prev, PROC_LIST_RUNQUEUE);
	return (previous);
}

uint8_t scheduler_run(proc_t* task) {
	if (task->status >= PROC_ZOMBIE) {
		return (1);
	}
	task->status = PROC_RUN;
	list_add(&task->run_lst, &runqueue);
	return (0);
}

uint8_t scheduler_sleep(proc_t* task) {
	(void)task;
	printk("sleeping\n");
	return (0);
}

uint8_t scheduler_stopped(proc_t* task) {
	(void)task;
	printk("stopped\n");
	return (0);
}
uint8_t scheduler_zombie(proc_t* task) {
	(void)task;
	return (0);
}

uint8_t scheduler_dead(proc_t* task) {
	if (task->status != PROC_DEAD) {
		return (1);
	}
	list_extract(task);
	remove_tss_descriptor(task->gdt_index);
	free_process(task);
	return (0);
}

uint8_t scheduler_remove_task(proc_t* task) {
	if (task->status != PROC_DEAD) {
		return (1);
	}
	family_shrinking(task);
	list_extract(task);
	free_process(task);
	return (0);
}

proc_t* scheduler_get_current_proc() {
	return (current);
}

proc_t* scheduler_get_proc_by_pid(uint16_t pid) {
	list_head_t* tmp = tasklist.next;
	proc_t*		 proc = NULL;
	while (tmp != &tasklist) {
		if (((proc_t*)tmp)->pid == pid) {
			proc = (proc_t*)tmp;
			break;
		}
		tmp = tmp->next;
	}
	return (proc);
}

list_head_t* scheduler_get_tasklist() {
	return (&tasklist);
}

void family_growing(proc_t* task) {
	list_add(&task->siblings, &task->parent->childrens);
}

void family_shrinking(proc_t* task) {
	list_del(&task->siblings);
}

void family_adopt_orphans(proc_t* task) {
	proc_t*		 proc = scheduler_get_proc_by_pid(0);
	list_head_t* current = task->childrens.next;

	while (current != &task->childrens) {
		list_add(&current, &proc->childrens);
		current = current->next;
	}
}

uint16_t getpid() {
	return (current->pid);
}

uint16_t getppid() {
	return (current->parent->pid);
}
