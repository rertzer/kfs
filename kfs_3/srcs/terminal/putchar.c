#include "keycode.h"
#include "terminal.h"

#include "utils.h" //for getting strlen

extern terminal_t term;

size_t term_putstr(const char* str) {
	size_t len = strlen(str);
	len = term_putchars(str, len);
	return (len);
}

size_t term_putchars(const char* array, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		term_putchar(array[i]);
	}
	return (len);
}

size_t term_putchars_ln(const char* array, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		term_putchar(array[i]);
	}
	term_putchar('\n');
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
			term_backspace();
			break;
		default:
			vga_write(c, term.color, term.column, term.row);
			term_next();
			break;
	}

	return ((size_t)1);
}
