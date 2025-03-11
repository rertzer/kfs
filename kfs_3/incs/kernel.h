#ifndef KERNEL_H
#define KERNEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"
#include "pic.h"
#include "printk.h"

#define KBRD_INTRFC 0x64
#define KBRD_RESET 0xFE
#define KBRD_UDATA 0x02

/* ======================== assembly functions ==================================== */
void	 init_pit();
void	 reset_timer_counter();
uint32_t get_timer_counter();
void	 godot();
void	 invalidate_low_kernel();
void	 flush_tlb();
uint8_t	 kbrd_reboot();
uint8_t	 qemu_shutdown();
void	 get_registers(uint32_t* registers);

/* ======================== utils ==================================== */
size_t	 strlen(const char* str);
void	 ft_memset(uint8_t* address, char c, uint32_t size);
uint32_t round_up_power_two(uint32_t n);
void	 page_testing();
bool	 is_alnum(uint8_t c);
uint32_t index_len(uint32_t start, uint32_t end);
// uint32_t min(uint32_t a, uint32_t b);

/* ======================== kernel utils ============================== */
unsigned char inb(unsigned short port);
void		  outb(unsigned char value, unsigned short port);
void		  outw(uint16_t value, uint16_t port);
void		  io_wait(void);
void		  sleep();
uint32_t*	  to_upper_kernel(uint32_t* low_address);
void		  int_allowed();

#endif
