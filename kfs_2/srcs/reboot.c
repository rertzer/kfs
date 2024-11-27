#include "builtin.h"
#include "kernel.h"

/*
 * kreboot reboot the computer using the 8042 keyboard controller
 * it pulse de CPU's RESET pin
 * */
uint8_t reboot() {
	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0xFE, 0x64);
	sleep();
	return (1);
}

/* reboot by triggering a triple fault. */
uint8_t kreboot() {
	freboot();
	return (1);
}

uint8_t halt() {
	outw(0x2000, 0x604);
	return (1);
}
