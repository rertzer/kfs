#include "builtin.h"
#include "kfs_list_head.h"
#include "processus.h"
#include "scheduler.h"
#include "stdlib.h"

uint8_t ps(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	list_head_t* task_list = scheduler_get_tasklist();
	list_for_each(task_list, print_process, PROC_LIST_LST);
	return (0);
}

uint8_t ps_info(size_t argc, char** argv) {
	if (argc != 2) {
		printk("psinfo requires a PID as argument\n");
		return (1);
	}
	uint16_t pid = atoi(argv[1]);
	proc_print_info(pid);
	return (0);
}
