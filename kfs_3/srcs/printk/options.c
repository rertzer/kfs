#include "printk.h"

int	is_num(int c) {
	return (c >= '0' && c <= '9');
}

printk_opts_t get_opts(va_list *l_args, const char* format, int i) {
	printk_opts_t opts = {0};
	int			  y = i;

	if (ft_strrchr(CONV, format[y + 1]) != 0) {
		opts.counter = 0;
		return (opts);
	}

	while (ft_strrchr(CONV, format[y + 1]) == 0 && (format[y + 1] < '1' || format[y + 1] > '9')) {
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
			case '*':
				opts.width = va_arg(*l_args, int);
				y++;
				break;
			default:
				opts.error = 1;
				y++;
				break;
		}
	}
	if (is_num(format[y + 1])) {
		opts.width = ft_atoi(&format[y + 1]);
		while (is_num(format[y + 1])) {
			y++;
		}
	}

	if (ft_strrchr(CONV, format[y + 1]) == 0) {
		opts.error = 1;
	}

	opts.counter = y - i;
	return (opts);
}
