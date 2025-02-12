#include "builtin.h"
#include "kernel.h"

/*
 * kreboot reboot the computer using the 8042 keyboard controller
 * it pulse de CPU's RESET pin
 * */
uint8_t reboot(char* pointer, size_t len) {
	(void)pointer;
	uint8_t counter = 0;
	printk("rebooting ");
	if (len == 0) {
		len = 8;
	}
	while (counter < len) {
		sleep();
		uint32_t time = get_timer_counter();
		if (time % 1000 == 0) {
			printk(".", time);
			++counter;
		}
	}

	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0xFE, 0x64);
	sleep();
	return (1);
}

/* reboot by triggering a triple fault. */
uint8_t kreboot(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	freboot();
	return (1);
}

uint8_t halt(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	outw(0x2000, 0x604);
	return (1);
}
