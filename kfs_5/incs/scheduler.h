#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "processus.h"
#include "tss.h"

void	scheduler_init(proc_t* proc_zero);
uint8_t scheduler_add_task(proc_t* task);
uint8_t scheduler_remove_task(proc_t* task);
uint8_t scheduler_run(proc_t* task);
void	increase_family(proc_t* task);
void	decrease_family(proc_t* task);
#endif
