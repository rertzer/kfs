#include "kernel.h"
#include "boot_infos.h"
#include "builtin.h"
#include "exec.h"
#include "exit.h"
#include "fork.h"
#include "gdt.h"
#include "interrupts.h"
#include "keycode.h"
#include "malloc.h"
#include "memory.h"
#include "scheduler.h"
#include "signal.h"
#include "terminal.h"
#include "tss.h"
#include "unistd.h"
#include "wait.h"

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_pit();
	init_idt();
	invalidate_low_kernel();
	init_memory();
	init_v_memory();
	run_task_zero();
}

void kernel_zero() {
	interrupts_allowed();
	int error = 0;
	int pid = fork();
	if (pid == 0) {
		exec(shell, 0, NULL);
	} else {
		exec(watchdog, 0, NULL);  // process 0 watchdog
	}
}
