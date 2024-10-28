#include "kernel.h"

uint8_t current_scancode = 0;

void keyboard_handler(uint8_t scan) {
	static uint8_t scan_status = SCAN_DEFAULT;

	current_scancode = scan;
	switch (scan_status) {
		case SCAN_DEFAULT:
			scan_status = handle_scancode_default();
			break;
		case SCAN_EXTENDED:
			scan_status = handle_scancode_extended();
			break;
		case SCAN_PAUSE:
			scan_status = handle_scancode_pause();
			break;
	}
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
