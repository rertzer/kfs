#include "wait.h"
#include "kfs_list_head.h"
#include "panic.h"
#include "processus.h"
#include "scheduler.h"
#include "unistd.h"

static proc_t*	find_zombie(list_head_t* kids, uint16_t pid);
static uint16_t zombie_found(proc_t* zombie, int* wstatus);

uint16_t wait(int* wstatus) {
	proc_t*	 current_proc = scheduler_get_current_proc();
	proc_t*	 zombie = NULL;
	uint16_t zombie_pid = 0;

	while (true) {
		if ((zombie = find_zombie(&current_proc->childrens, 0)) != NULL) {
			if (wstatus != NULL) {
				*wstatus = zombie->exit_status;
			}
			zombie_pid = zombie->pid;
			break;
		}
		pause();
	}
	zombie->status = PROC_DEAD;
	if (scheduler_dead(zombie) != 0) {
		panic("Cannot delete a non-Zombie process");
	}
	return (zombie_pid);
}

uint16_t waitpid(uint16_t pid, int* wstatus, int options) {
	proc_t*	 current_proc = scheduler_get_current_proc();
	proc_t*	 zombie = NULL;
	uint16_t zombie_pid = 0;

	while (true) {
		if ((zombie = find_zombie(&current_proc->childrens, pid)) != NULL) {
			zombie_found(zombie, wstatus);
			break;
		}
		if (options & WNOHANG) {
			break;
		}
		pause();
	}

	return (zombie_pid);
}

static proc_t* find_zombie(list_head_t* kids, uint16_t pid) {
	proc_t*		 zombie = NULL;
	list_head_t* kid = kids->next;

	while (kid != kids) {
		proc_t* child = list_get(kid, PROC_LIST_SIBLINGS);
		if (child->status == PROC_ZOMBIE && (pid <= 0 || child->pid == pid)) {
			zombie = child;
			break;
		}
		kid = kid->next;
	}
	return (zombie);
}

static uint16_t zombie_found(proc_t* zombie, int* wstatus) {
	if (wstatus != NULL) {
		*wstatus = zombie->exit_status;
	}
	zombie->status = PROC_DEAD;
	if (scheduler_dead(zombie) != 0) {
		panic("Cannot delete a non-Zombie process");
	}
	return (zombie->pid);
}
