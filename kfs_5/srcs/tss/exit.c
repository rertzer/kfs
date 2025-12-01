#include "exit.h"
#include "scheduler.h"
#include "signal.h"

void _exit(int status) {
	scheduler_set_current_status(PROC_ZOMBIE);
	proc_t* current_proc = scheduler_get_current_proc();
	// remove user stack
	current_proc->exit_status = status;
	// transfer kids !!!
	signal_sending(current_proc->parent, SIGCHLD);
}
