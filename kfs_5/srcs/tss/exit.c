#include "exit.h"
#include "printk.h"
#include "scheduler.h"
#include "signal.h"

void _exit(int status) {
	proc_t* current_proc = scheduler_get_current_proc();
	current_proc->exit_status = status;
	printk("exit %d with status %d\n", current_proc->pid, status);
	scheduler_enter();
	// remove user stack
	family_adopt_orphans(current_proc);
	scheduler_set_current_status(PROC_ZOMBIE);
	signal_sending(current_proc->parent, SIGCHLD);
	scheduler_switch_status();
	scheduler_switch_task(true);
	scheduler_leave();
}
