#include "keycode.h"
#include "string.h"
#include "terminal.h"

extern terminal_t term;

size_t term_putstr(char const* const str) {
	return (term_putchars(str, strlen(str)));
}

size_t term_putchars(const char* const array, size_t const len) {
	for (size_t i = 0; i < len; ++i) {
		term_putchar(array[i]);
	}
	return (len);
}

size_t term_putchars_ln(const char* const array, size_t const len) {
	for (size_t i = 0; i < len; ++i) {
		term_putchar(array[i]);
	}
	term_putchar('\n');
	return (len);
}

size_t term_putchar(char const c) {
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
