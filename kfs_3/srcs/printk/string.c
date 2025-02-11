#include "terminal.h"

int print_char(char c, printk_opts_t opts) {
	int len = 1;

	if (opts.width > len && opts.left == 0) {
		for (int i = 0; i < opts.width - len; i++)
			len += term_putchar(' ');
	}
	term_putchar(c);
	if (opts.width > len && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			len += term_putchar(' ');
	}
	return (len);
}

int print_string(char* str, printk_opts_t opts) {
	if (!str) {
		str = "(null)";
	}
	int len = strlen(str);
	int final_len = len;

	if (opts.width > len && opts.left == 0) {
		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(' ', (printk_opts_t){0});
	}

	while (*str)
		print_char(*str++, opts);

	if (opts.width > len && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(' ', (printk_opts_t){0});
	}

	return (final_len);
}
