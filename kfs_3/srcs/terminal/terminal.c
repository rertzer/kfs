#include "terminal.h"
#include "kernel.h"

terminal_t term;
terminal_t all_terms[MAX_TERM_NB];
uint16_t   all_terms_buffers[MAX_TERM_NB][VGA_WIDTH * VGA_HEIGHT];
size_t	   current_term;

static void			 term_init(size_t const term_index);
static void			 term_init_buffer(size_t const i);
static void			 term_set_header(size_t const i);
static void			 term_set_prompt(size_t const i);
static inline size_t term_prompt_line_end();
static inline void	 load_term_buffer(uint16_t* const dest, uint16_t const* const src);
static void			 term_init_values(size_t i);
static void			 term_set_buffer(size_t i, char c);
static inline bool	 term_at_prompt_left_border();
static inline bool	 term_at_lower_line_left_border();

void all_terms_init() {
	for (size_t i = 0; i < MAX_TERM_NB; ++i) {
		term_init(i);
	}
	term.buffer = (uint16_t*)VGA_TEXT_MODE_BUFFER;
	current_term = 0;
	load_term(&term, &all_terms[current_term]);
	update_cursor(term.column, term.row);
	printk("Welcome to jrOS!\n");
	printk("- terminal init OK\n");
}

void term_init(size_t const i) {
	term_init_values(i);
	term_init_buffer(i);
}

static void term_init_values(size_t i) {
	all_terms[i].row = PRINT_ROW_START;
	all_terms[i].column = PROMPT_SIZE + 1;
	all_terms[i].prompt_row = PRINT_ROW_START;
	all_terms[i].prompt_column = PROMPT_SIZE + 1;
	all_terms[i].line_len = 2;
	all_terms[i].left_margin = 0;
	all_terms[i].color = vga_char_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	all_terms[i].buffer = all_terms_buffers[i];
}

static void term_init_buffer(size_t const i) {
	term_set_buffer(i, ' ');
	term_set_header(i);
	term_set_prompt(i);
}

static void term_set_buffer(size_t i, char c) {
	uint16_t const vc = vga_char(c, all_terms[i].color);
	uint16_t const vhc = vga_char(c, vga_char_color(VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY));

	for (size_t x = 0; x < VGA_WIDTH; ++x) {
		size_t index = x;
		for (size_t y = 0; y < VGA_HEIGHT; ++y) {
			all_terms[i].buffer[index] = (y <= JROS_HEADER_HIGH) ? vhc : vc;
			index += VGA_WIDTH;
		}
	}
}

static void term_set_header(size_t const i) {
	uint8_t const jros_header[8][JROS_HEADER_HIGH][JROS_HEADER_WIDTH] = {
		{JROS_HEADER_0}, {JROS_HEADER_1}, {JROS_HEADER_2}, {JROS_HEADER_3},
		{JROS_HEADER_4}, {JROS_HEADER_5}, {JROS_HEADER_6}, {JROS_HEADER_7}};

	for (size_t y = 0; y < JROS_HEADER_HIGH; ++y) {
		for (size_t x = 0; x < JROS_HEADER_WIDTH; ++x) {
			const size_t index = VGA_WIDTH * y + x + JROS_HEADER_OFFSET;
			all_terms[i].buffer[index] =
				vga_char(jros_header[i][y][x], vga_char_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_DARK_GREY));
		}
	}
}

static void term_set_prompt(size_t const i) {
	size_t const index = all_terms[i].row * VGA_WIDTH;
	all_terms[i].buffer[index] = vga_char(PROMPT, vga_char_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
}

void load_term(terminal_t* const dest, terminal_t const* const src) {
	dest->row = src->row;
	dest->column = src->column;
	dest->prompt_row = src->prompt_row;
	dest->prompt_column = src->prompt_column;
	dest->line_len = src->line_len;
	dest->color = src->color;
	dest->left_margin = src->left_margin;
	load_term_buffer(dest->buffer, src->buffer);
}

static inline void load_term_buffer(uint16_t* const dest, uint16_t const* const src) {
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
		dest[i] = src[i];
	}
}

void switch_term(size_t const next) {
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

void term_prompt() {
	term.line_len = 0;
	term.prompt_row = term.row;
	vga_write(PROMPT, vga_char_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK), term.column, term.row);
	term_next();
	term_next();
}

size_t term_prompt_pos() {
	return (term.prompt_row * VGA_WIDTH + term.prompt_column);
}

size_t term_cursor_pos() {
	return (term.row * VGA_WIDTH + term.column);
}

void write_tab() {
	for (size_t i = 0; i < TAB_SIZE; ++i) {
		vga_write(' ', term.color, term.column, term.row);
		term_next();
	}
}

void term_up() {
	term_left();
}

void term_down() {
	term_right();
}

bool term_left() {
	bool moved = false;
	if (!term_at_prompt_left_border() && !term_at_lower_line_left_border()) {
		--term.column;
		moved = true;
		update_cursor(term.column, term.row);
	} else if (term_at_lower_line_left_border()) {
		--term.row;
		term.column = VGA_WIDTH - 1;
		moved = true;
		update_cursor(term.column, term.row);
	}
	return (moved);
}

static inline bool term_at_prompt_left_border() {
	return (term.row == term.prompt_row && term.column <= term.prompt_column);
}

static inline bool term_at_lower_line_left_border() {
	return (term.row > term.prompt_row && term.column == 0);
}

bool term_right() {
	bool   moved = false;
	size_t current_index = (term.row - term.prompt_row) * VGA_WIDTH + term.column;
	if (current_index < term.line_len) {
		if (term.column != VGA_WIDTH - 1) {
			++term.column;
		} else {
			++term.row;
			term.column = 0;
		}
		moved = true;
		update_cursor(term.column, term.row);
	}
	return (moved);
}

void term_home() {
	term.row = term.prompt_row;
	term.column = term.prompt_column;
	update_cursor(term.column, term.row);
}

void term_end() {
	term.row = term.prompt_row + (term.line_len / VGA_WIDTH);
	term.column = term.line_len % VGA_WIDTH;
	update_cursor(term.column, term.row);
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

void term_backspace() {
	if (term_left()) {
		size_t current = term_cursor_pos();
		size_t end = term_prompt_line_end();
		if (current == end) {
			vga_write(' ', term.color, term.column, term.row);
		} else {
			for (; current < end; ++current) {
				term.buffer[current] = term.buffer[current + 1];
			}
		}
		--term.line_len;
	}
}

static inline size_t term_prompt_line_end() {
	return (term.prompt_row * VGA_WIDTH + term.line_len);
}

void term_delete() {
	size_t current = term.row * VGA_WIDTH + term.column;
	size_t end = term.prompt_row * VGA_WIDTH + term.line_len;
	if (current < end) {
		for (; current < end; ++current) {
			term.buffer[current] = term.buffer[current + 1];
		}
		--term.line_len;
	}
}

void term_previous() {
	--term.line_len;
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
	++term.line_len;
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
	--term.prompt_row;
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
	term.column = term.left_margin;
	if (term.row == VGA_HEIGHT - 1) {
		term_scroll();
	} else {
		++term.row;
	}
	update_cursor(term.column, term.row);
}

void update_cursor(size_t x, size_t y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x0E, (uint16_t)0x3D4);
	outb((uint8_t)((pos >> 8) & 0xFF), (uint16_t)0x3D5);
	outb(0x0F, (uint16_t)0x3D4);
	outb((uint8_t)(pos & 0xFF), (uint16_t)0x3D5);
}
