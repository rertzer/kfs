#include "kernel.h"

extern uint8_t stack_bottom;

void hexdump(uint8_t *data, uint32_t length) {
	uint32_t i;
	for (i = 0; i < length; i++) {
		if (i % 16 == 0) {
			printk("\n");
		}
		printk("%02x ", data[i]);
	}
	printk("\n");
}