#include "wait.h"
#include "kfs_list_head.h"
#include "processus.h"
#include "scheduler.h"
#include "unistd.h"

static proc_t* find_zombie(list_head_t* kids);

uint16_t wait(int* wstatus) {
	proc_t*	 current_proc = scheduler_get_current_proc();
	proc_t*	 zombie = NULL;
	uint16_t zombie_pid = 0;

	while (true) {
		pause();
		if ((zombie = find_zombie(&current_proc->childrens)) != NULL) {
			if (wstatus != NULL) {
				*wstatus = zombie->exit_status;
			}
			zombie_pid = zombie->pid;
			break;
		}
	}
	scheduler_set_current_status(PROC_DEAD);
	return (zombie_pid);
}

static proc_t* find_zombie(list_head_t* kids) {
	proc_t*		 zombie = NULL;
	list_head_t* current = kids->next;

	while (current != kids) {
		proc_t* kid = list_get(current, PROC_LIST_CHILDRENS);
		if (kid->status == PROC_ZOMBIE) {
			zombie = kid;
			break;
		}
		current = current->next;
	}
	return (zombie);
}
