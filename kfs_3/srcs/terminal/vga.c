#include "vga.h"

size_t vga_linear_position(size_t x, size_t y) {
	return (y * VGA_WIDTH + x);
}

void vga_write(uint16_t* vga_buffer, char c, uint8_t color, size_t x, size_t y) {
	vga_buffer[vga_linear_position(x, y)] = vga_char(c, color);
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
