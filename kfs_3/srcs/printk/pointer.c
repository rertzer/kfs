#include "printk.h"

static int print_addr(unsigned long int ptr) {
	int len = 0;
	if (ptr >= 16) {
		len += print_addr(ptr / 16);
		len += print_addr(ptr % 16);
	}
	if (ptr < 16) {
		if (ptr < 10)
			len += print_char(ptr + '0', (printk_opts_t){0});
		else
			len += print_char(ptr + 87, (printk_opts_t){0});
	}
	return (len);
}

int print_pointer(unsigned long int ptr, printk_opts_t opts) {
	if (ptr == 0)
		return (print_string("(nil)", opts));

	int len = get_hexa_len(ptr) + 2;
	int len_final = len;

	if (opts.width > len && opts.left == 0) {
		if (opts.zero == 0) {
			for (int i = 0; i < opts.width - len; i++)
				len_final += print_char(' ', (printk_opts_t){0});
			print_string("0x", (printk_opts_t){0});
			print_addr(ptr);
		} else {
			print_string("0x", (printk_opts_t){0});
			for (int i = 0; i < opts.width - len; i++)
				len_final += print_char('0', (printk_opts_t){0});
			print_addr(ptr);
		}
	}

	if (opts.width > len && opts.left == 1) {
		print_string("0x", (printk_opts_t){0});
		print_addr(ptr);
		for (int i = 0; i < opts.width - len; i++)
			len_final += print_char(' ', (printk_opts_t){0});
	}

	return (len_final);
}
