#ifndef PROCESSUS_H
#define PROCESSUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kfs_bitmap.h"
#include "list_head.h"

#include "tss.h"

#define MAX_PROC_NB 1024
#define MAX_PID 32767
#define PID_BITMAP_SIZE ((MAX_PID + 1) / BITMAP_BITS_PER_ENTRY)

#define PROC_LIST_LST (0)
#define PROC_LIST_RUNQUEUE (LIST_HEAD_SIZE)
#define PROC_LIST_CHILDRENS (2 * LIST_HEAD_SIZE)
#define PROC_LIST_SIBLINGS (3 * LIST_HEAD_SIZE)

typedef enum { PROC_RUN, PROC_SLEEP, PROC_STOPPED, PROC_ZOMBIE, PROC_DEAD } proc_status_e;

typedef struct signal_lst_s {
	list_head_t lst;
	uint32_t	signal;
} signal_lst_t;

typedef struct proc_lst_s {
	list_head_t	   lst;
	struct proc_s* proc;
} proc_lst_t;

typedef struct proc_s {
	list_head_t	   lst;
	list_head_t	   run_lst;
	list_head_t	   childrens;
	list_head_t	   siblings;
	struct proc_s* father;
	uint32_t	   pid;
	uint32_t	   owner;
	tss_t*		   tss;
	signal_lst_t*  signals;
	uint8_t*	   stack;
	uint8_t*	   heap;
	proc_status_e  status;

} proc_t;

void	 pid_bitmap_init();
uint32_t pid_bitmap_get_new();
void	 pid_bitmap_remove(uint32_t pid);
void	 pid_bitmap_test();

proc_t	init_zero_proc();
uint8_t spawn(proc_t* src, proc_t* dest);
void	free_process(proc_t* task);
#endif
