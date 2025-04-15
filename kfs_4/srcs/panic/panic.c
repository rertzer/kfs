#include "panic.h"
#include "builtin.h"
#include "kernel.h"
#include "terminal.h"

extern terminal_t term;

volatile char* dump = (char*)0xC0110000;

static void		   print_panic(char* msg);
static void		   panic_set_term_background();
static void		   panic_set_header();
static void		   panic_set_cursor(size_t const r, size_t const c);
static inline void panic_set_left_margin(size_t const m);
static void		   panic_init_values();

void panic(char* msg) {
	dump_stack();
	print_panic(msg);
	int_allowed();
	reboot(NULL, 20);
}

static void print_panic(char* msg) {
	panic_set_term_background();
	panic_set_header();
	panic_init_values();
	term_putstr("Ooops!\n");
	term_putchar(*dump);
	panic_set_cursor(9, 28);
	term_putstr("Something bad happened :(\n");
	panic_set_cursor(10, 12);
	term_putstr(msg);
	panic_set_left_margin(22);
	term_putstr("\n\n");
	registers(NULL, 0);
	panic_set_left_margin(12);
	panic_set_cursor(23, 12);
}

static void panic_set_term_background() {
	uint16_t const vc = vga_char(' ', vga_char_color(VGA_COLOR_GREEN, VGA_COLOR_RED));

	for (size_t x = 0; x < VGA_WIDTH; ++x) {
		size_t index = x;
		for (size_t y = 0; y < VGA_HEIGHT; ++y) {
			term.buffer[index] = vc;
			index += VGA_WIDTH;
		}
	}
}

static void panic_set_header() {
	uint8_t const panic_header[PANIC_HEADER_HIGH][PANIC_HEADER_WIDTH] = {PANIC_HEADER};

	for (size_t y = 0; y < PANIC_HEADER_HIGH; ++y) {
		for (size_t x = 0; x < PANIC_HEADER_WIDTH; ++x) {
			const size_t index = VGA_WIDTH * y + x + PANIC_HEADER_OFFSET;
			term.buffer[index] = vga_char(panic_header[y][x], vga_char_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_RED));
		}
	}
}

static void panic_set_cursor(size_t const r, size_t const c) {
	term.row = r;
	term.column = c;
}

static inline void panic_set_left_margin(size_t const m) {
	term.left_margin = m;
}

static void panic_init_values() {
	term.row = 8;
	term.column = 36;
	term.prompt_row = 0;
	term.prompt_column = 0;
	term.line_len = 22;
	term.left_margin = 12;
	term.color = vga_char_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_RED);
}
