#include "unistd.h"

#include "keycode.h"
#include "terminal.h"
extern terminal_t term;

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
			term_write(c, term.color, term.column, term.row);
			term_next();
			break;
	}

	return ((size_t)1);
}

int	write(int fd, char* buffer, size_t buffer_size) {
	(void)fd;
	for (size_t i = 0; i < buffer_size; ++i) {
		term_putchar(buffer[i]);
	}
	return (buffer_size);
}