#ifndef KERNEL_H
#define KERNEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"
#include "pic.h"
#include "printk.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_TEXT_MODE_BUFFER 0xc00B8000
#define GDT_BUFFER 0xc0000800

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

/* ============= GDT ============= */

typedef struct {
	uint32_t base;
	uint32_t limit;
	uint8_t	 access;
	uint8_t	 flags;
} gdt_descriptor_t;

typedef union gdt_entry_u {
	uint64_t desc;
	uint8_t	 bytes[8];
} gdt_entry_t;

void init_gdt();

// assembly functions
void	 set_gdt(uint32_t limit, uint32_t base);
void	 init_pit();
void	 reset_timer_counter();
uint32_t get_timer_counter();
void	 godot();
void	 invalidate_low_kernel();
void	 flush_tlb();

/* ======================== utils ==================================== */
size_t	 strlen(const char* str);
uint8_t	 hexdump(char* pointer, size_t len);
void	 tabledump();
void	 ft_memset(uint8_t* address, char c, uint32_t size);
uint32_t round_up_power_two(uint32_t n);
void	 page_testing();
/* ======================== utils ===================================== */
size_t	 strlen(const char* str);
bool	 is_alnum(uint8_t c);
uint32_t index_len(uint32_t start, uint32_t end);
uint32_t min(uint32_t a, uint32_t b);
/* ======================== kernel utils ============================== */
unsigned char inb(unsigned short port);
void		  outb(unsigned char value, unsigned short port);
void		  outw(uint16_t value, uint16_t port);
void		  io_wait(void);
void		  sleep();
uint32_t*	  to_upper_kernel(uint32_t* low_address);
/* ======================= keyboard =================================== */
#define LOG_INFO "INFO "
#define LOG_WARNING "WARNING "
#define LOG_ERROR "ERROR "

unsigned char get_input();
void		  term_next_line();
unsigned char get_keyboard_input();
int			  printk(const char* format, ...);

/* ======================== scancode ====================================== */
typedef struct {
	uint8_t pressed : 1;
	uint8_t control : 1;
	uint8_t alt : 1;
	uint8_t shift : 1;
	uint8_t gui : 1;
	uint8_t capslock : 1;
	uint8_t numlock : 1;
	uint8_t scrolllock : 1;
	uint8_t scancode;
	uint8_t ascii;
	uint8_t keycode;
} keypress_t;

typedef void (*handle_fun_t)(keypress_t* k);
/* ====================== readline ========================================= */
typedef struct {
	char   cmd[256];
	size_t cmd_len;
	char   arg[256];
	size_t arg_len;
} cmdline_t;

bool readline();

#endif
