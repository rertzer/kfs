#ifndef VGA_H
#define VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_TEXT_MODE_BUFFER 0xc00B8000

#define MAX_TERM_NB 8

#define TRUE 1
#define FALSE 0
#define TAB_SIZE 4

typedef enum vga_color_e {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_t;

typedef struct {
	uint8_t fg : 4;
	uint8_t bg : 4;
} char_color_t;

typedef union {
	uint8_t		 color;
	char_color_t char_color;
} char_color_u;

typedef enum vga_blink_e {
	NO_BLINK = 0,
	BLINK = 1,
} vga_blink_t;

uint8_t		vga_char_color(vga_color_t fg, vga_color_t bg);
uint16_t	vga_char(unsigned char uc, uint8_t color);
vga_color_t vga_fg_color(uint8_t color);
vga_color_t vga_bg_color(uint8_t color);

void		vga_write(uint16_t* vga_buffer, char c, uint8_t color, size_t x, size_t y);

#endif
