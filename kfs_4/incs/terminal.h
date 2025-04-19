#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel.h"
#include "vga.h"

#define PROMPT_SIZE 1
#define PROMPT '>'

#define JROS_HEADER_HIGH 5
#define JROS_HEADER_WIDTH 30
#define JROS_HEADER_OFFSET 25

#define PRINT_ROW_START JROS_HEADER_HIGH + 1

#define JROS_HEADER_0                                                                                         \
	"      _     ____  ____  ___   ", "     (_)___/ __ \\/ __/ / _ \\  ", "    / / __/ /_/ /\\ \\  / // /  ", \
		" __/ /_/  \\____/___/  \\___/   ", "|___/                         "

#define JROS_HEADER_1                                                                                        \
	"      _     ____  ____  ___   ", "     (_)___/ __ \\/ __/ <  /   ", "    / / __/ /_/ /\\ \\   / /    ", \
		" __/ /_/  \\____/___/  /_/     ", "|___/                         "

#define JROS_HEADER_2                                                                                     \
	"      _     ____  ____  ___ ", "     (_)___/ __ \\/ __/ |_  | ", "    / / __/ /_/ /\\ \\  / __/   ", \
		" __/ /_/  \\____/___/ /____/   ", "|___/                         ",

#define JROS_HEADER_3                                                                                        \
	"      _     ____  ____  ____  ", "     (_)___/ __ \\/ __/ |_  /  ", "    / / __/ /_/ /\\ \\  _/_ <   ", \
		" __/ /_/  \\____/___/ /____/   ", "|___/                         ",

#define JROS_HEADER_4                                                                                       \
	"      _     ____  ____   ____ ", "     (_)___/ __ \\/ __/  / / /", "    / / __/ /_/ /\\ \\  /_  _/  ", \
		" __/ /_/  \\____/___/   /_/    ", "|___/                         ",

#define JROS_HEADER_5                                                                                         \
	"      _     ____  ____  ____  ", "     (_)___/ __ \\/ __/ / __/  ", "    / / __/ /_/ /\\ \\  /__ \\   ", \
		" __/ /_/  \\____/___/ /____/   ", "|___/                         ",

#define JROS_HEADER_6                                                                                         \
	"      _     ____  ____  ____  ", "     (_)___/ __ \\/ __/ / __/  ", "    / / __/ /_/ /\\ \\  / _ \\   ", \
		" __/ /_/  \\____/___/  \\___/   ", "|___/                         ",

#define JROS_HEADER_7                                                                                       \
	"      _     ____  ____  ____ ", "     (_)___/ __ \\/ __/ /_  /  ", "    / / __/ /_/ /\\ \\    / /   ", \
		" __/ /_/  \\____/___/   /_/    ", "|___/                         ",

typedef struct terminal_s {
	uint16_t* buffer;
	size_t	  row;
	size_t	  column;
	size_t	  prompt_row;
	size_t	  prompt_column;
	size_t	  line_len;
	size_t	  left_margin;
	uint8_t	  color;
} terminal_t;

void		vga_write(char c, uint8_t color, size_t x, size_t y);
uint8_t		vga_char_color(vga_color_t fg, vga_color_t bg);
uint16_t	vga_char(unsigned char uc, uint8_t color);
vga_color_t vga_fg_color(uint8_t color);
vga_color_t vga_bg_color(uint8_t color);
void		all_terms_init();
void		load_term(terminal_t* const dest, terminal_t const* const src);
void		switch_term(size_t const next);
void		switch_next_term();
void		switch_previous_term();
void		term_backspace();
void		term_delete();
void		term_up();
void		term_down();
bool		term_left();
bool		term_right();
void		term_home();
void		term_end();
void		term_first_column();
void		term_last_column();
void		term_previous_row();
void		term_next_row();
void		term_previous();
void		term_next();
void		term_scroll();
void		term_front_color_next();
void		term_back_color_next();
size_t		term_putstr(const char* const str);
size_t		term_putchars(const char* const array, size_t const len);
size_t		term_putchars_ln(const char* const array, size_t const len);
size_t		term_putchar(char const c);
void		term_prompt();
size_t		term_prompt_pos();
size_t		term_cursor_pos();
size_t		term_line_len();
void		write_tab();
void		update_cursor(size_t x, size_t y);
void		term_next_line();
#endif
