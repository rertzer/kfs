#include "printk.h"

int print_integer(long long int n) {
	int len = 0;
	if (n < 0) {
		n = -n;
		len += print_char('-', (printk_opts_t){0});
	}
	if (n > 9) {
		len += print_integer(n / 10);
		len += print_char(n % 10 + '0', (printk_opts_t){0});
	} else
		len += print_char(n + '0', (printk_opts_t){0});
	return (len);
}

int prep_print_integer(long long int n, printk_opts_t opts) {
	int len = nbr_len(n);
	int final_len = len;

	if (opts.sign)
		opts.blank = 0;

	if (opts.sign == 1 && n >= 0) {
		final_len = ++len;
	}

	if (opts.blank == 1 && n >= 0) {
		final_len = ++len;
		print_char(' ', (printk_opts_t){0});
	}

	if (opts.sign == 1 && n >= 0 && opts.zero == 1) {
		print_char('+', (printk_opts_t){0});
	}

	if (opts.width > len && opts.left == 0) {
		char c = opts.zero ? '0' : ' ';

		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(c, (printk_opts_t){0});
	}

	if (opts.sign == 1 && n >= 0 && opts.zero == 0) {
		print_char('+', (printk_opts_t){0});
	}

	print_integer(n);

	if (opts.width > len && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(' ', (printk_opts_t){0});
	}

	return (final_len);
}
