#include "builtin.h"
#include "kernel.h"
#include "utils.h"

static uint8_t counter_clock();

uint8_t reboot(size_t argc, char** argv) {
	size_t time = 8;
	if (argc > 1) {
		time = ft_atoi(argv[1]);
	}

	printk("rebooting ");
	while (counter_clock() <= time) {
		sleep();
	}

	kbrd_reboot();
	return (1);
}

static uint8_t counter_clock() {
	static uint8_t counter;

	if (get_timer_counter() % 1000 == 0) {
		printk(".");
		++counter;
	}
	return (counter);
}

uint8_t halt(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	qemu_shutdown();
	return (1);
}
