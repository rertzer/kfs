#ifndef PRINTK_H
#define PRINTK_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "character_set.h"

#define FLAG "0-+ #"
#define CONV "%cspdiuxXn"

typedef struct {
	uint8_t zero : 1;
	uint8_t left : 1;
	uint8_t sign : 1;
	uint8_t blank : 1;
	uint8_t prefix : 1;
	uint8_t error : 1;
	int		counter;
	int		width;
} printk_opts_t;

//=============== utils =================
size_t ft_atoi(const char* str);
size_t nbr_len(int nbr);
int	   get_hexa_len(unsigned int nbr);
size_t ft_strrchr(const char* str, char c);
bool   is_alpha(char c);
bool   is_digit(char c);

//=============== conv =================
int print_char(char c, printk_opts_t opts);
int print_string(char* str, printk_opts_t opts);
int print_hexa_maj(unsigned int nbr, printk_opts_t opts);
int print_hexa_min(unsigned int nbr, printk_opts_t opts);
int prep_print_integer(long long int n, printk_opts_t opts);
int print_pointer(unsigned long int ptr, printk_opts_t opts);
int prep_print_unsigned(unsigned int n, printk_opts_t opts);

//=============== options =================
printk_opts_t get_opts(va_list *l_args, const char* format, int i);

//=============== printk =================
int printk(const char* format, ...);

#endif
