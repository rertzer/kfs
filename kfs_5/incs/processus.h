#ifndef PROCESSUS_H
#define PROCESSUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tss.h"

#define MAX_PROC_NB 1024

typedef enum { READY, RUN, SLEEP, ZOMBIE } proc_status_e;

typedef struct signal_lst_s {
	struct signal_lst_s* prev;
	struct signal_lst_s* next;
	uint32_t			 signal;
} signal_lst_t;

typedef struct proc_s {
	uint32_t		pid;
	uint32_t		owner;
	struct proc_s*	father;
	struct procs_t* childrens;
	tss_t*			tss;
	signal_lst_t*	signals;
	proc_status_e	status;
	uint8_t*		stack;
	uint8_t*		heap;

} proc_t;

proc_t init_zero_proc();
#endif
