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

void update_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x0E, (uint16_t)0x3D4);
	outb((uint8_t)((pos >> 8) & 0xFF), (uint16_t)0x3D5);
	outb(0x0F, (uint16_t)0x3D4);
	outb((uint8_t)(pos & 0xFF), (uint16_t)0x3D5);
}
