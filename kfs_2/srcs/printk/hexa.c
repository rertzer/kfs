#include "kernel.h"

static int write_hexa(unsigned int nbr, char* base) {
	int len = 0;
	if (nbr >= 16)
		len += write_hexa(nbr / 16, base);
	len += print_char(base[nbr % 16], (printk_opts_t){0});
	return (len);
}

int print_hexa_maj(unsigned int nbr, printk_opts_t opts) {
	int len = get_hexa_len(nbr);
	int final_len = len;
	
	if (opts.width > get_hexa_len(nbr) && opts.left == 0) {
		char c = opts.zero ? '0' : ' ';

		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(c, (printk_opts_t){0});
	}
	write_hexa(nbr, "0123456789ABCDEF");

	if (opts.width > get_hexa_len(nbr) && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(' ', (printk_opts_t){0});
	}

	return (final_len);
}

int print_hexa_min(unsigned int nbr, printk_opts_t opts) {
	int len = get_hexa_len(nbr);
	int final_len = len;
	
	if (opts.prefix == 1 && nbr != 0) {
		final_len = len += 2;
	}

	if (opts.prefix == 1 && nbr != 0 && opts.zero == 1)
		print_string("0x", (printk_opts_t){0});

	if (opts.width > len && opts.left == 0) {
		char c = opts.zero ? '0' : ' ';

		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(c, (printk_opts_t){0});
	}

	if (opts.prefix == 1 && nbr != 0 && opts.zero == 0)
		print_string("0x", (printk_opts_t){0});

	write_hexa(nbr, "0123456789abcdef");

	if (opts.width > get_hexa_len(nbr) && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(' ', (printk_opts_t){0});
	}

	return (final_len);
}