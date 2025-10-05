#include "builtin.h"
#include "list_head.h"
#include "processus.h"
#include "scheduler.h"

uint8_t ps(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	list_head_t* task_list = scheduler_get_tasklist();
	list_for_each(task_list, print_process, PROC_LIST_LST);
	return (0);
}
