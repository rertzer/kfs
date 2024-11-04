#ifndef PRINTK_H
#define PRINTK_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint8_t zero : 1;
	uint8_t left : 1;
	uint8_t sign : 1;
	uint8_t blank : 1;
	uint8_t prefix : 1;
	int		width;

} printk_opts_t;

size_t	ft_atoi(const char *str);
size_t nbr_len(int nbr);
int get_hexa_len(unsigned int nbr);

int		print_char(char c, printk_opts_t opts);
int		print_string(char* str, printk_opts_t opts);
int		print_hexa_maj(unsigned int nbr, printk_opts_t opts);
int		print_hexa_min(unsigned int nbr, printk_opts_t opts);
int		prep_print_integer(long long int n, printk_opts_t opts);
int		print_pointer(unsigned long int ptr, printk_opts_t opts);
int		prep_print_unsigned(unsigned int n, printk_opts_t opts);

printk_opts_t get_opts(const char* format, int* i);

int		printk(const char* format, ...);

#endif