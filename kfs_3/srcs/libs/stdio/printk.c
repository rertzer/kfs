#include "printk.h"
#include "unistd.h"

int printk(const char* format, ...) {
	int		output_size;
	va_list	l_args;

	va_start(l_args, format);
	output_size = vdprintf(STD_OUT, format, l_args);
	va_end(l_args);
	return (output_size);
}