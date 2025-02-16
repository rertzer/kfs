#include "printk.h"

int print_unsigned(unsigned int n) {
	int len = 0;

	if (n > 9) {
		len += print_unsigned(n / 10);
		len += print_char(n % 10 + '0', (printk_opts_t){0});
	} else
		len += print_char(n + '0', (printk_opts_t){0});
	return (len);
}

int prep_print_unsigned(unsigned int n, printk_opts_t opts) {
	int len = nbr_len(n);
	int len_final = len;

	if (opts.width > len && opts.left == 0) {
		char c = opts.zero ? '0' : ' ';

		for (int i = 0; i < opts.width - len; i++)
			len_final += print_char(c, (printk_opts_t){0});
	}

	print_unsigned(n);

	if (opts.width > len && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			len_final += print_char(' ', (printk_opts_t){0});
	}

	return (len_final);
}
