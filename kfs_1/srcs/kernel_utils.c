#include "kernel.h"

unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(port) : "memory");
	return ret;
}

void outb(unsigned char value, unsigned short port) {
	asm volatile("out %%al, %%dx" : : "a"(value), "d"(port) : "memory");
}

void io_wait(void) {
	outb(0, 0x80);
}

void sleep() {
	__asm__ volatile("hlt");
}
