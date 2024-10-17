#include "kernel.h"

terminal_t term;

void kernel_main(void) {
	term_init();
	init_gdt();
	term_putstr("42");

	while (true) {
		uint8_t	   scancode = get_keyboard_input();
		keypress_t keypress = handle_scancode(scancode);
		if (keypress.pressed == TRUE && keypress.ascii != 0) {
			term_putchar(keypress.ascii);
		}
	}
}

void term_init() {
	term.row = 0;
	term.column = 0;
	term.color = vga_char_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	term.buffer = (uint16_t*)VGA_TEXT_MODE_BUFFER;
	term_set_buffer(' ');
	// enable_cursor(0, 25);
	update_cursor(term.column, term.row);
}

void term_set_buffer(char c) {
	uint16_t vc = vga_char(c, term.color);
	for (size_t x = 0; x < VGA_WIDTH; ++x) {
		for (size_t y = 0; y < VGA_HEIGHT; ++y) {
			const size_t index = VGA_WIDTH * y + x;
			term.buffer[index] = vc;
		}
	}
}

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

size_t term_putstr(const char* str) {
	size_t len = strlen(str);
	for (size_t i = 0; i < len; ++i) {
		term_putchar(str[i]);
	}

	return (len);
}

size_t term_putchar(char c) {
	switch (c) {
		case '\t':
			write_tab();
			break;
		case '\n':
			term_next_line();
			break;
		case BACKSPACE:
			term_previous();
			vga_write(' ', term.color, term.column, term.row);
			break;
		default:
			vga_write(c, term.color, term.column, term.row);
			term_next();
			break;
	}

	return ((size_t)1);
}

void write_tab() {
	for (size_t i = 0; i < TAB_SIZE; ++i) {
		vga_write(' ', term.color, term.column, term.row);
		term_next();
	}
}
void term_previous() {
	if (term.column == 0) {
		if (term.row != 0) {
			term.column = 79;
			--term.row;
		}
	} else {
		--term.column;
	}
	update_cursor(term.column, term.row);
}

void term_next() {
	++term.column;
	if (term.column == VGA_WIDTH) {
		term.column = 0;
		++term.row;
		if (term.row == VGA_HEIGHT) {
			term.row = 0;
		}
	}
	update_cursor(term.column, term.row);
}

void term_next_line() {
	term.column = 0;
	++term.row;
	if (term.row == VGA_HEIGHT) {
		term.row = 0;
	}
	update_cursor(term.column, term.row);
}

size_t strlen(const char* str) {
	size_t len = 0;

	while (str[len]) {
		++len;
	}
	return (len);
}
