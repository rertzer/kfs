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

unsigned char get_keyboard_input() {
	while (!(inb(0x64) & 0x1))
		;
	return inb(0x60);
}
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x0A, (uint16_t)0x3D4);
	outb((inb(0x3D5) & 0xC0) | cursor_start, (uint16_t)0x3D5);

	outb(0x0B, (uint16_t)0x3D4);
	outb((inb(0x3D5) & 0xE0) | cursor_end, (uint16_t)0x3D5);
}
void update_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x0E, (uint16_t)0x3D4);
	outb((uint8_t)((pos >> 8) & 0xFF), (uint16_t)0x3D5);
	outb(0x0F, (uint16_t)0x3D4);
	outb((uint8_t)(pos & 0xFF), (uint16_t)0x3D5);
}
