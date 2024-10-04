#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_TEXT_MODE_BUFFER 0xB8000
#define TRUE 1
#define FALSE 0

typedef struct terminal_s {
  uint16_t *buffer;
  size_t row;
  size_t column;
  uint8_t color;
} terminal_t;

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

typedef enum vga_blink_e {
  NO_BLINK = 0,
  BLINK = 1,
} vga_blink_t;

size_t strlen(const char *str);
void vga_write(char c, uint8_t color, size_t x, size_t y);
uint8_t vga_char_color(vga_color_t fg, vga_color_t bg);
uint16_t vga_char(unsigned char uc, uint8_t color);
void term_next();
void term_init();
void term_set_buffer(char c);
void term_putstr(const char *str);
void term_putchar(char c);
#endif
