#ifndef KERNEL_H
#define KERNEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"
#include "pic.h"
#include "printk.h"

// assembly functions
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

#endif
