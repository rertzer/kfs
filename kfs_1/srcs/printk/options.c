#include "kernel.h"

printk_opts_t get_opts(const char* format, int i) {
	printk_opts_t opts = {0};
	int y = i;

	if (ft_strrchr(CONV, format[y + 1]) != 0) {
		opts.counter = 0;
		return (opts);
	}

	while ((format[y + 1] < '1' || format[y + 1] > '9')) {
		switch (format[y + 1]) {
			case '+':
				opts.sign = 1;
				y++;
				break;
			case '0':
				opts.zero = 1;
				y++;
				break;
			case '-':
				opts.left = 1;
				y++;
				break;
			case ' ':
				opts.blank = 1;
				y++;
				break;
			case '#':
				opts.prefix = 1;
				y++;
				break;
			default:
				opts.error = 1;
				y++;
				break;
			}
		}
		if (format[y + 1] >= '0' && format[y + 1] <= '9') {
			opts.width = ft_atoi(&format[y + 1]);
			while (format[y + 1] >= '0' && format[y + 1] <= '9') {
				y++;
			}
		}

		if (ft_strrchr(CONV, format[y + 1]) == 0) {
			opts.error = 1;
			term_putstr("Error: invalid conv\n");
		}

	opts.counter = y - i;
	return (opts);
}