#ifndef PROCESSUS_H
#define PROCESSUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "fork.h"
#include "kfs_bitmap.h"
#include "list_head.h"

#include "tss.h"

#define MAX_PROC_NB 1024
#define MAX_PID 32767
#define PID_BITMAP_SIZE ((MAX_PID + 1) / BITMAP_BITS_PER_ENTRY)

#define KERNEL_STACK_SIZE (4 * PAGE_SIZE)
#define KERNEL_STACK_HIGH_MASK (0x3FFF)

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
	list_head_t	   heap;
	void*		   kernel_stack;
	struct proc_s* parent;
	uint32_t	   owner;
	uint32_t	   gdt_index;
	tss_t*		   tss;
	signal_lst_t*  signals;
	int16_t		   pid;
	proc_status_e  status;

} proc_t;

void	pid_bitmap_init();
int16_t pid_bitmap_get_new();
void	pid_bitmap_remove(int16_t pid);
void	pid_bitmap_test();

proc_t*	 init_zero_proc();
proc_t*	 spawn(proc_t* src, fork_data_t fd);
void	 proc_set_gdt_index(proc_t* task, uint32_t gdt_index);
void	 free_process(proc_t* task);
void	 print_process(void* vtask);
void	 copy_stack(fork_data_t fd, uint8_t* low_stack);
uint8_t* get_kernel_stack_high(uint8_t* sp);
#endif
