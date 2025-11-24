#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "processus.h"
#include "tss.h"

void		 scheduler();
void		 scheduler_switch_task();
void		 scheduler_switch_status();
void		 scheduler_init(proc_t* proc_zero);
uint8_t		 scheduler_add_task(proc_t* task);
uint8_t		 scheduler_remove_task(proc_t* task);
uint8_t		 scheduler_set_current_status(proc_status_e status);
uint8_t		 scheduler_change_status(proc_t* task);
proc_t*		 scheduler_unrun(proc_t* task);
uint8_t		 scheduler_run(proc_t* task);
uint8_t		 scheduler_sleep(proc_t* task);
uint8_t		 scheduler_stopped(proc_t* task);
uint8_t		 scheduler_zombie(proc_t* task);
uint8_t		 scheduler_dead(proc_t* task);
proc_t*		 scheduler_get_current_proc();
proc_t*		 scheduler_get_proc_by_pid(uint16_t pid);
list_head_t* scheduler_get_tasklist();
void		 family_growing(proc_t* task);
void		 family_shrinking(proc_t* task);
uint16_t	 getpid();
#endif
