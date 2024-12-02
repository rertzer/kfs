#include "kernel.h"

int printk(const char* format, ...) {
	va_list l_args;
	int		len = 0;

	va_start(l_args, format);

	for (int i = 0; format[i]; i++) {
		if (format[i] == '%')
		{
			printk_opts_t opts = get_opts(format, i);

			if (opts.error) {
				while(!is_alpha(format[i]))
					len += print_char(format[i++], (printk_opts_t){0});
				len += print_char(format[i], (printk_opts_t){0});				
				continue;
			}

			i += opts.counter;
			switch (format[++i]) {
				case '%':
					len += print_char('%', opts);
					break;
				case 'c':
					len += print_char(va_arg(l_args, int), opts);
					break;
				case 's':
					len += print_string(va_arg(l_args, char*), opts);
					break;
				case 'p':
					len += print_pointer(va_arg(l_args, unsigned long), opts);
					break;
				case 'd':
					len += prep_print_integer(va_arg(l_args, int), opts);
					break;
				case 'i':
					len += prep_print_integer(va_arg(l_args, int), opts);
					break;
				case 'u':
					len += prep_print_unsigned(va_arg(l_args, unsigned int), opts);
					break;
				case 'x':
					len += print_hexa_min(va_arg(l_args, unsigned int), opts);
					break;
				case 'X':
					len += print_hexa_maj(va_arg(l_args, unsigned int), opts);
					break;
			}

		} else
			len += print_char(format[i], (printk_opts_t){0});
	}
	va_end(l_args);
	return (len);
}
