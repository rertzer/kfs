#include "kernel.h"

terminal_t term;
terminal_t all_terms[MAX_TERM_NB];
uint16_t   all_terms_buffers[MAX_TERM_NB][VGA_WIDTH * VGA_HEIGHT];
size_t	   current_term;

void all_terms_init() {
	for (size_t i = 0; i < MAX_TERM_NB; ++i) {
		term_init(i);
	}
	// enable_cursor(0, 25);
	term.buffer = (uint16_t*)VGA_TEXT_MODE_BUFFER;
	current_term = 0;
	load_term(&term, &all_terms[current_term]);
	update_cursor(term.column, term.row);
}

void term_init(size_t i) {
	all_terms[i].row = 0;
	all_terms[i].column = 0;
	all_terms[i].color = vga_char_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	all_terms[i].buffer = all_terms_buffers[i];
	term_set_buffer(i, ' ');
}

void term_set_buffer(size_t i, char c) {
	uint16_t vc = vga_char(c, all_terms[i].color);
	for (size_t x = 0; x < VGA_WIDTH; ++x) {
		for (size_t y = 0; y < VGA_HEIGHT; ++y) {
			const size_t index = VGA_WIDTH * y + x;
			all_terms[i].buffer[index] = vc;
		}
	}
}

void load_term(terminal_t* dest, terminal_t* src) {
	dest->row = src->row;
	dest->column = src->column;
	dest->color = src->color;
	load_term_buffer(dest->buffer, src->buffer);
}

void load_term_buffer(uint16_t* dest, uint16_t* src) {
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
		dest[i] = src[i];
	}
}

void switch_term(size_t next) {
	load_term(&all_terms[current_term], &term);
	load_term(&term, &all_terms[next]);
	current_term = next;
	update_cursor(term.column, term.row);
}

void switch_next_term() {
	size_t next = current_term + 1;
	if (next == MAX_TERM_NB) {
		next = 0;
	}
	switch_term(next);
}

void switch_previous_term() {
	size_t next = current_term;
	if (next == 0) {
		next = MAX_TERM_NB - 1;
	} else {
		--next;
	}
	switch_term(next);
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

void term_up() {
	if (term.column != 0) {
		--term.column;
	}
	update_cursor(term.column, term.row);
}

void term_down() {
	if (term.column != VGA_HEIGHT) {
		++term.column;
	}
	update_cursor(term.column, term.row);
}

void term_left() {
	if (term.column != 0) {
		--term.column;
		update_cursor(term.column, term.row);
	}
}

void term_right() {
	if (term.column != VGA_WIDTH - 1) {
		++term.column;
		update_cursor(term.column, term.row);
	}
}

void term_fisrt_column() {
	term.column = 0;
	update_cursor(term.column, term.row);
}

void term_last_column() {
	term.column = VGA_WIDTH;
	update_cursor(term.column, term.row);
}
void term_previous_row() {
	if (term.row != 0) {
		--term.row;
	}
	update_cursor(term.column, term.row);
}

void term_next_row() {
	if (term.row != VGA_HEIGHT - 1) {
		++term.row;
	}
	update_cursor(term.column, term.row);
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
		if (term.row == VGA_HEIGHT - 1) {
			term_scroll();
		} else {
			++term.row;
		}
	}
	update_cursor(term.column, term.row);
}

void term_scroll() {
	size_t	 index;
	uint16_t vc = vga_char(' ', term.color);
	for (index = 0; index < VGA_WIDTH * (VGA_HEIGHT - 1); ++index) {
		term.buffer[index] = term.buffer[index + VGA_WIDTH];
	}
	for (; index < VGA_WIDTH * VGA_HEIGHT; ++index) {
		term.buffer[index] = vc;
	}
}

void term_front_color_next() {
	char_color_u color;
	color.color = term.color;
	if (color.char_color.fg == VGA_COLOR_WHITE) {
		color.char_color.fg = VGA_COLOR_BLACK;

	} else {
		++color.char_color.fg;
	}
	term.color = color.color;
}

void term_back_color_next() {
	char_color_u color;
	color.color = term.color;
	if (color.char_color.bg == VGA_COLOR_WHITE) {
		color.char_color.bg = VGA_COLOR_BLACK;

	} else {
		++color.char_color.bg;
	}
	term.color = color.color;
}

void term_next_line() {
	term.column = 0;
	if (term.row == VGA_HEIGHT - 1) {
		term_scroll();
	} else {
		++term.row;
	}
	update_cursor(term.column, term.row);
}
