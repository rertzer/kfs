#include "exec.h"
#include "builtin.h"
#include "exit.h"
#include "keycode.h"
#include "printk.h"
#include "processus.h"
#include "scheduler.h"
#include "unistd.h"

void exec_fn(exec_fun_t fun, size_t argc, char** argv) {
	uint16_t pid = fork();
	int		 error = 0;
	if (pid == 0) {
		printk("ready to exec\n");
		press_any();
		if ((error = exec_asm(fun, argc, argv)) != 0) {
			printk("exec failed\n");
			_exit(error);
		}
		_exit(0);
	}
}

uint8_t test_exec(size_t argc, char** argv) {
	(void)argv;
	while (true) {
		printk("hello from test exec, argc is %d\n", argc);
		scheduler();
	}
}

uint8_t test_exec_2(size_t argc, char** argv) {
	(void)argv;
	printk("hello from test exec 2, argc is %d\n", argc);
	press_any();
	printk("Good bye!\n");
	return (0);
}
