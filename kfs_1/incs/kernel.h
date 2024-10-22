#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "keycode.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_TEXT_MODE_BUFFER 0x000B8000
#define GDT_BUFFER 0x00000800

#define MAX_TERM_NB 8

#define TRUE 1
#define FALSE 0
#define TAB_SIZE 4

typedef struct terminal_s {
	uint16_t* buffer;
	size_t	  row;
	size_t	  column;
	uint8_t	  color;
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

/* ============= GDT ============= */

#define GDT_BUFFER 0x00000800

#define GDT_SIZE 7
#define GDT_TABLE \
	{0,		0,			0,		0},\
	{0,		0xFFFFF,	0x9A,	0xC},\
	{0,		0xFFFFF,	0x92,	0xC},\
	{0,		0xFFFFF,	0x96,	0xC},\
	{0,		0xFFFFF,	0xFA,	0xC},\
	{0,		0xFFFFF,	0xF2,	0xC},\
	{0,		0xFFFFF,	0xF6,	0xC},\
//   base 	limit		access	flags

typedef struct {
	uint16_t limit;
	uint32_t base;
}  __attribute__((packed)) gdt_ptr_t;

typedef struct {
	uint32_t base;
	uint32_t limit;
	uint8_t	 access;
	uint8_t	 flags;
} gdt_descriptor_t;

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t	 base_middle;
	uint8_t	 access_byte;
	uint8_t	 limit_high_flags;
	uint8_t	 base_high;
} __attribute__((packed)) gdt_entry_t;

void init_gdt();

// assembly functions
void gdt_flush(uint32_t gdt_ptr);


/* ============= IDT ============= */

#define IDT_MAX_DESCRIPTORS 32
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_32BIT_INTERRUPT 0x0E

typedef struct {
	uint16_t limit;
	uint32_t base;
}  __attribute__((packed)) idt_ptr_t;

typedef struct {
	uint32_t isr;
	uint8_t	 flags;
} idt_descriptor_t;

typedef struct {
	uint16_t isr_low;	  // Lower 16 bits of the ISR's address
	uint16_t kernel_cs;	  // GDT segment selector loaded into CS
	uint8_t	 reserved;	  // Set to zero
	uint8_t	 attributes;  // Flags
	uint16_t isr_high;	  // Higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;


void init_idt();
void exception_handler(void);

// assembly functions
void idt_flush(uint32_t idt_ptr);

/* ======================== utils ==================================== */
size_t strlen(const char* str);

/* ========================= terminal ================================ */
#define LOG_INFO "INFO "
#define LOG_WARNING "WARNING "
#define LOG_ERROR "ERROR "

unsigned char inb(unsigned short port);
void		  outb(unsigned char value, unsigned short port);
unsigned char get_input();
void		  term_next_line();
unsigned char get_keyboard_input();
void		  enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void		  update_cursor(size_t x, size_t y);
int			  printk(const char* format, ...);

/* ======================= keyboard =================================== */
#define LOG_INFO "INFO "
#define LOG_WARNING "WARNING "
#define LOG_ERROR "ERROR "

void		vga_write(char c, uint8_t color, size_t x, size_t y);
uint8_t		vga_char_color(vga_color_t fg, vga_color_t bg);
uint16_t	vga_char(unsigned char uc, uint8_t color);
vga_color_t vga_fg_color(uint8_t color);
vga_color_t vga_bg_color(uint8_t color);
void		all_terms_init();
void		term_init(size_t term_index);
void		load_term(terminal_t* dest, terminal_t* src);
void		load_term_buffer(uint16_t* dest, uint16_t* src);
void		switch_term(size_t next);
void		switch_next_term();
void		switch_previous_term();
void		term_up();
void		term_down();
void		term_left();
void		term_right();
void		term_first_column();
void		term_last_column();
void		term_previous_row();
void		term_next_row();
void		term_previous();
void		term_next();
void		term_scroll();
void		term_front_color_next();
void		term_back_color_next();
void		write_tab();

void		term_set_buffer(size_t i, char c);
size_t		term_putstr(const char* str);
size_t		term_putchar(char c);

/* ======================== scancode ====================================== */
typedef struct {
	uint8_t extended : 1;
	uint8_t pressed : 1;
	uint8_t control : 1;
	uint8_t alt : 1;
	uint8_t shift : 1;
	uint8_t capslock : 1;
	uint8_t numlock : 1;
	uint8_t scrolllock : 1;
	uint8_t scancode;
	uint8_t ascii;
	uint8_t keycode;
} keypress_t;

typedef void (*handle_fun_t)(keypress_t* k);

keypress_t handle_scancode(uint8_t scancode);
void	   handle_control(keypress_t* current);
void	   handle_shift(keypress_t* current);
void	   handle_alt(keypress_t* current);
void	   handle_capslock(keypress_t* current);
void	   handle_numlock(keypress_t* current);
void	   handle_scrolllock(keypress_t* current);

/* ===================== keycode ======================================= */
void handle_keypress(keypress_t* keypress);
void handle_home();
// void handle_up(keypress_t* keypress);
// void handle_page_up(keypress_t* keypress);
void handle_left(keypress_t* keypress);
void handle_right(keypress_t* keypress);
void handle_end();
// void handle_down(keypress_t* keypress);
// void handle_page_down(keypress_t* keypress);
void handle_delete();
void handle_control_ascii(uint8_t c);

#endif
