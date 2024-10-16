#include <kernel.h>

unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(port) : "memory");
	return ret;
}

void outb(unsigned char value, unsigned short port) {
	asm volatile("out %%al, %%dx" : : "a"(value), "d"(port) : "memory");
}

unsigned char get_keyboard_input() {
	while (!(inb(0x64) & 0x1))
		;
	return inb(0x60);
}
