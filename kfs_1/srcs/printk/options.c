#include "kernel.h"

printk_opts_t get_opts(const char* format, int* i) {
	printk_opts_t opts = {0};

	while (
		format[*i + 1] == '+' || 
		format[*i + 1] == '0' || 
		format[*i + 1] == '-' ||
		format[*i + 1] == ' ' ||
		format[*i + 1] == '#') {
			switch (format[*i + 1]) {
				case '+':
					opts.sign = 1;
					(*i)++;
					break;
				case '0':
					opts.zero = 1;
					(*i)++;
					break;
				case '-':
					opts.left = 1;
					(*i)++;
					break;
				case ' ':
					opts.blank = 1;
					(*i)++;
					break;
				case '#':
					opts.prefix = 1;
					(*i)++;
					break;
			}
		}
		if (format[*i + 1] >= '0' && format[*i + 1] <= '9') {
			opts.width = ft_atoi(&format[*i + 1]);
			while (format[*i + 1] >= '0' && format[*i + 1] <= '9') {
				(*i)++;
			}
		}
	return (opts);
}