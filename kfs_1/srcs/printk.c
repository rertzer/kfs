#include "kernel.h"

int	print_char(char c) {
	return (term_putchar(c));
}

int	print_string(char *str) {
	int	len = 0; 

	if (!str)
		return (term_putstr("(null)"));
	while (*str)
		len += print_char(*str++);
	return (len);
}

static int	write_hexa(unsigned int nbr, char *base) {
	int	len = 0;
	if (nbr >= 16)
		len += write_hexa(nbr / 16, base);
	len += print_char(base[nbr % 16]);
	return (len);
}

int	print_hexa_maj(unsigned int nbr) {
	return (write_hexa(nbr, "0123456789ABCDEF"));
}

int	print_hexa_min(unsigned int nbr) {
	return (write_hexa(nbr, "0123456789abcdef"));
}

int	print_integer(long long int n) {
	int	len = 0;
	if (n < 0) {
		n = -n;
		len += print_char('-');
	}
	if (n > 9) {
		len += print_integer(n / 10);
		len += print_char(n % 10 + '0');
	}
	else
		len += print_char(n + '0');
	return (len);
}

static int	print_addr(unsigned long int ptr) {
	int	len = 0;
	if (ptr >= 16) {
		len += print_addr(ptr / 16);
		len += print_addr(ptr % 16);
	}
	if (ptr < 16) {
		if (ptr < 10)
			len += print_char(ptr + '0');
		else
			len += print_char(ptr + 87);
	}
	return (len);
}

int	print_pointer(unsigned long int ptr) {
	int	len = 0;
	if (ptr == 0)
		return (print_string("(nil)"));
	len += print_string("0x");
	len += print_addr(ptr);
	return (len);
}


int	print_unsigned(unsigned int n) {
	int	len = 0;

	if (n > 9) {
		len += print_unsigned(n / 10);
		len += print_char(n % 10 + '0');
	}
	else
		len += print_char(n + '0');
	return (len);
}

int	check_format(char form, va_list l_args) {
	int	len = 0;

	switch (form) {
		case '%':
			len += print_char('%');
			break;
		case 'c':
			len += print_char(va_arg(l_args, int));
			break;
		case 's':
			len += print_string(va_arg(l_args, char *));
			break;
		case 'p':
			len += print_pointer(va_arg(l_args, unsigned long));
			break;
		case 'd':
			len += print_integer(va_arg(l_args, int));
			break;
		case 'i':
			len += print_integer(va_arg(l_args, int));
			break;
		case 'u':
			len += print_unsigned(va_arg(l_args, unsigned int));
			break;
		case 'x':
			len += print_hexa_min(va_arg(l_args, unsigned int));
			break;
		case 'X':
			len += print_hexa_maj(va_arg(l_args, unsigned int));
			break;	
	}
	return (len);
}

int	printk(const char *format, ...) {
	va_list	l_args;
	int		len = 0;

	va_start(l_args, format);

	for (int i = 0; format[i]; i++) {
		if (format[i] == '%')
			len += check_format(format[++i], l_args);
		else
			len += print_char(format[i]);
	}
	va_end(l_args);
	return (len);
}