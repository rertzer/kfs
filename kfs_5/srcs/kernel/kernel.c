#include "kernel.h"
#include "builtin.h"
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

extern volatile uint8_t current_code;

static void process_keyboard(keypress_t* keypress);

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
	char x = 'X';

	printk("jrOS ready. Welcome to kernel zero. Enjoy!\n");
	keypress_t keypress = init_keypress();
	term_prompt();
	uint16_t tr = store_task_register();
	printk("TR: %08x %d\n", tr);

	uint16_t pid = fork();
	printf("child pid : %d\n", pid);
	pid = fork();
	printf("returned pid : %d\n", pid);
	// print_gdt();
	if (pid == 2) {
		scheduler();
		printf("%d! I am your father\n", pid);
		uint16_t tr = store_task_register();
		printk(" father TR: %08x %c\n", tr, x);
	} else {
		printf("I am %d, your son\n", pid);
		uint16_t tr = store_task_register();
		printk("son TR: %08x  %c\n", tr, x);
		scheduler();
	}

	kill(2, 9);
	scheduler();

	while (true) {
		sleep();
		process_keyboard(&keypress);
	}
}

static void process_keyboard(keypress_t* keypress) {
	bool getline = false;
	keypress->keycode = current_code;
	current_code = 0;
	if (keypress->keycode != 0) {
		update_keypress(keypress);
		if (keypress->pressed == PRESSED) {
			getline = handle_keypress(*keypress);
		}
	}
	if (getline == true) {
		process_line();
		term_prompt();
	}
}
