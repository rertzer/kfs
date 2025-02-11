#include "kernel.h"
#include "terminal.h"

extern terminal_t term;

void vga_write(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	term.buffer[index] = vga_char(c, color);
}

uint16_t vga_char(unsigned char uc, uint8_t color) {
	return ((uint16_t)uc | (uint16_t)color << 8);
}

uint8_t vga_char_color(vga_color_t fg, vga_color_t bg) {
	return (fg | bg << 4);
}

vga_color_t vga_fg_color(uint8_t color) {
	return (color & 0x0F);
}

vga_color_t vga_bg_color(uint8_t color) {
	return ((color & 0xF0) >> 4);
}
