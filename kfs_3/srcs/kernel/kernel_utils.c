#include "kernel.h"

extern uint32_t* HIGH_SHIFT;

unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm volatile("in %%dx, %%al" : "=a"(ret) : "d"(port) : "memory");
	return ret;
}

void outb(unsigned char value, unsigned short port) {
	asm volatile("out %%al, %%dx" : : "a"(value), "d"(port) : "memory");
}

void outw(uint16_t value, uint16_t port) {
	asm volatile("outw %%ax, %%dx" : : "a"(value), "d"(port) : "memory");
}
void io_wait(void) {
	outb(0, 0x80);
}

void sleep() {
	__asm__ volatile("hlt");
}

void int_allowed() {
	__asm__ volatile("sti");
}

uint32_t* to_upper_kernel(uint32_t* low_address) {
	uint32_t* high_address = low_address + 0xC0000000 / 4;
	return (high_address);
}

uint8_t kbrd_reboot() {
	uint8_t flags = KBRD_UDATA;
	while (flags & KBRD_UDATA) {
		flags = inb(KBRD_INTRFC);
	}
	outb(KBRD_RESET, KBRD_INTRFC);
	sleep();
	return (1);
}
uint8_t qemu_shutdown() {
	outw(0x2000, 0x604);
	return (1);
}
