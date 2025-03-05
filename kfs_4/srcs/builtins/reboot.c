#include "builtin.h"
#include "kernel.h"

static uint8_t counter_clock();

uint8_t reboot(char* pointer, size_t len) {
	(void)pointer;
	if (len == 0) {
		len = 8;
	}

	printk("rebooting ");
	while (counter_clock() <= len) {
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

uint8_t halt(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	qemu_shutdown();
	return (1);
}
