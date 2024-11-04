#include "kernel.h"

typedef struct {
	uint8_t zero : 1;
	uint8_t left : 1;
	uint8_t sign : 1;
	int		width;

} printk_opts_t;

size_t	ft_atoi(const char *str);
size_t nbr_len(size_t nbr);
int		print_char(char c, printk_opts_t opts);
int		print_string(char* str, printk_opts_t opts);
int		print_hexa_maj(unsigned int nbr, printk_opts_t opts);
int		print_hexa_min(unsigned int nbr, printk_opts_t opts);
int		prep_print_integer(long long int n, printk_opts_t opts);
int		print_pointer(unsigned long int ptr, printk_opts_t opts);
int		prep_print_unsigned(unsigned int n, printk_opts_t opts);
int		printk(const char* format, ...);

size_t	ft_atoi(const char *str) {
	size_t i = 0;
	size_t res = 0;
	size_t sign = 1;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		++i;
	if (str[i] == '-' || str[i] == '+') {
		++i;
		if (str[i - 1] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9') {
		res = res * 10 + str[i] - '0';
		++i;
	}
	return (res * sign);
}

size_t nbr_len(size_t nbr) {
	size_t len = 0;
	if (nbr > 9) {
		len += nbr_len(nbr / 10);
		len += 1;
	} else
		len += 1;
	return (len);
}

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

static int write_hexa(unsigned int nbr, char* base) {
	int len = 0;
	if (nbr >= 16)
		len += write_hexa(nbr / 16, base);
	len += print_char(base[nbr % 16], (printk_opts_t){0});
	return (len);
}

int get_hexa_len(unsigned int nbr) {
	int len = 0;
	if (nbr >= 16)
		len += get_hexa_len(nbr / 16);
	return (len + 1);
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
	
	if (opts.width > len && opts.left == 0) {
		char c = opts.zero ? '0' : ' ';

		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(c, (printk_opts_t){0});
	}
	
	write_hexa(nbr, "0123456789abcdef");

	if (opts.width > get_hexa_len(nbr) && opts.left == 1) {
		for (int i = 0; i < opts.width - len; i++)
			final_len += print_char(' ', (printk_opts_t){0});
	}

	return (final_len);
}

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

	if (opts.sign == 1 && n > 0) {
		final_len = ++len;
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


//TODO: if flag not recognized, print the whole string
//Implemented: - + 0 width
//Not implemented: precision blank #

int printk(const char* format, ...) {
	va_list l_args;
	int		len = 0;

	va_start(l_args, format);

	for (int i = 0; format[i]; i++) {
		if (format[i] == '%')
		{
			printk_opts_t opts = {0};

			while (format[i + 1] == '+' || format[i + 1] == '0' || format[i + 1] == '-') {
				switch (format[i + 1]) {
					case '+':
						opts.sign = 1;
						i++;
						break;
					case '0':
						opts.zero = 1;
						i++;
						break;
					case '-':
						opts.left = 1;
						i++;
						break;
				}
			}

			if (format[i + 1] >= '0' && format[i + 1] <= '9') {
				opts.width = ft_atoi(&format[i + 1]);
				while (format[i + 1] >= '0' && format[i + 1] <= '9') {
					i++;
				}
			}

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
