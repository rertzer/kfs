#include "stdio.h"

#include <stdbool.h>
#include <stddef.h>
#include "unistd.h"
#include "utils.h"

#include "stdlib.h"

typedef struct s_data {
	char		*output_buffer;
	int			output_len;
	const char	*format;
	va_list		*l_args;

	struct {
		enum e_flags {
			ZERO	= 1 << 0,
			LEFT	= 1 << 1,
			SIGN	= 1 << 2,
			BLANK	= 1 << 3,
			PREFIX	= 1 << 4,
		} flags;
		int		width;
	}	options;
} t_state;

static enum e_flags	flags_map(char c) {
	switch (c) {
		case '+': return (SIGN);
		case '0': return (ZERO);
		case '-': return (LEFT);
		case ' ': return (BLANK);
		case '#': return (PREFIX);
	}
	return (0);
}

typedef void	(*t_fp_dump)(t_state*);

static void	dump_character(t_state *state, char character, size_t n) {
	ft_memset(
		&state->output_buffer[state->output_len],
		character,
		n);
	state->output_len += n;
}

static void dump_string(t_state *state, char *str, size_t len) {
	ft_memcpy(
		&state->output_buffer[state->output_len],
		str, len);
	state->output_len += len;
}

static void	_print_character(t_state *state) {
	char character = va_arg(*state->l_args, int);

	if (state->options.width > 1 && state->options.flags & ~LEFT) {
		dump_character(state, ' ', state->options.width - 1);
	}

	dump_character(state, character, 1);

	if (state->options.width > 1 && state->options.flags & LEFT) {
		dump_character(state, ' ', state->options.width - 1);
	}
}

static void _print_string(t_state *state) {
	char*	str		= va_arg(*state->l_args, char*);
	str = str ? str : "(null)";
	int	len	= strlen(str);

	if (state->options.width > len && state->options.flags & ~LEFT) {
		dump_character(state, ' ', state->options.width - len);
	}

	dump_string(state, str, len);

	if (state->options.width > len && state->options.flags & LEFT) {
		dump_character(state, ' ', state->options.width - len);
	}
}

static void _print_pointer(t_state *state) {
	unsigned long	address = va_arg(*state->l_args, unsigned long);
	if (address == 0) return ((void)dump_string(state, "(nil)", 5));

	char	tmp[20] = {};
	itoa(address, tmp, 16);
	int len = strlen(tmp);

	if (state->options.width > len && state->options.flags & ~LEFT) {
		if (state->options.flags & ZERO) {
			dump_string(state, "0x", 2);
			dump_character(state, '0', state->options.width - len);
		}
		else {
			dump_character(state, ' ', state->options.width - len);
			dump_string(state, "0x", 2);
		}
		dump_string(state, tmp, len);
	}

	if (state->options.width > len && state->options.flags & LEFT) {
		dump_string(state, "0x", 2);
		dump_string(state, tmp, len);
		dump_character(state, ' ', state->options.width - len);
	}
}

static void _print_integer(t_state *state) {
	int number = va_arg(*state->l_args, int);

	char	tmp[20] = {};
	itoa(number > 0 ? number : -number, tmp, 10);
	int len = strlen(tmp);


	if (state->options.flags & SIGN) state->options.flags &= ~BLANK;

	if (number >= 0) {
		if (state->options.flags & BLANK) {
			dump_character(state, ' ', 1);
		}
		if (state->options.flags & (SIGN | ZERO)) {
			dump_character(state, '+', 1);
		}
	}

	if (state->options.width > len && state->options.flags & ~LEFT) {
		dump_character(state,
			(state->options.flags & ZERO) ? '0' : ' ',
			state->options.width - len);
	}
	
	if (number < 0) {
		dump_character(state, '-', 1);
	}
	else if (state->options.flags & SIGN) {
		dump_character(state, '+', 1);
	}
	dump_string(state, tmp, len);

	if (state->options.width > len && state->options.flags & LEFT) {
		dump_character(state, ' ', state->options.width - len);
	}
}

static void _print_unsigned_integer(t_state *state) {
	unsigned int number = va_arg(*state->l_args, unsigned int);
	
	char	tmp[20] = {};
	itoa(number, tmp, 10);
	int len = strlen(tmp);

	if (state->options.width > len && state->options.flags & ~LEFT) {
		dump_character(state,
			(state->options.flags & ZERO) ? '0' : ' ',
			state->options.width - len);
	}

	dump_string(state, tmp, len);

	if (state->options.width > len && state->options.flags & LEFT) {
		dump_character(state, ' ', state->options.width - len);
	}
}

static void _print_lower_hexadecimal_case(t_state *state) {
	unsigned int number = va_arg(*state->l_args, unsigned long);
	char	tmp[20] = {};
	itoa(number, tmp, 16);
	int	len = strlen(tmp);

	if (number && state->options.flags & (PREFIX | ZERO)) {
		dump_string(state, "0x", 2);
	}

	if (state->options.width > len && state->options.flags & ~LEFT) {
		dump_character(state,
			(state->options.flags & ZERO) ? '0' : ' ',
			state->options.width - len);
	}

	if (number && state->options.flags & (PREFIX & ~ZERO)) {
		dump_string(state, "0x", 2);
	}

	dump_string(state, tmp, len);

	if (state->options.width > len && state->options.flags & LEFT) {
		dump_character(state, ' ', state->options.width - len);
	}
}

static void _print_upper_hexadecimal_case(t_state *state) {
	int	old_pos = state->output_len;	
	_print_lower_hexadecimal_case(state);
	for (int i = old_pos; i < state->output_len; i++) {
		state->output_buffer[i] = to_upper(state->output_buffer[i]);
	}
}

static void _print_percentage(t_state *state) {
	dump_character(state, '%', 1);
}

static void _print_current_len(t_state *state) {
	*(va_arg(*state->l_args, unsigned int*)) = state->output_len;
}

static t_fp_dump print_map(char c) {
	switch (c) {
		case 'c': return(_print_character);
		case 's': return(_print_string);
		case 'p': return(_print_pointer);
		case 'i': return(_print_integer);
		case 'd': return(_print_integer);
		case 'u': return(_print_unsigned_integer);
		case 'x': return(_print_lower_hexadecimal_case);
		case 'X': return(_print_upper_hexadecimal_case);
		case '%': return(_print_percentage);
		case 'n': return(_print_current_len);
	}
	return (NULL);
}

static bool parse_format(t_state *old) {
	t_fp_dump	print_function;

	if (*old->format != '%') return (false);
	t_state new = (*old);
	new.format += 1;
	new.options.flags = 0;
	new.options.width = 0;

	enum e_flags next_flag = flags_map(new.format[0]);
	while (next_flag) {
		new.options.flags |= next_flag;
		new.format += 1;
		next_flag = flags_map(new.format[0]);
	}
	
	// TODO: change it to strtol
	if (new.format[0] != '0' && is_digit(new.format[0])) {
		new.options.width = ft_atoi(new.format);
		while (new.format[0] && is_digit(new.format[0])) new.format += 1;
	}
	// TODO: check if the char after the '*' is okay before consuming with va_arg
	if (new.format[0] == '*') {
		new.options.width = va_arg(*new.l_args, int);
		new.format += 1;
	}
	print_function = print_map(new.format[0]);
	if (print_function == NULL) return (false);
	new.format += 1;
	(*old) = new;
	print_function(old);
	return (true);
}

int	vsprintf(char* buffer, const char* format, va_list l_args) {
	if (!buffer || !format) return (-1);

	t_state	state = {
		.output_buffer = buffer,
		.output_len = 0,
		.l_args = &l_args,
		.format = format,
	};

	while (*state.format) {
		if (parse_format(&state) == false) {
			dump_character(&state, *state.format, 1);
			state.format += 1;
		}
	}
	return (state.output_len);
}

int vdprintf(int fd, const char* format, va_list l_args) {
	static char	buffer[PRINTF_BUFFER_SIZE];
	int			len;

	len = vsprintf(buffer, format, l_args);
    if (len < 0) return len;
    return (write(fd, buffer, len));
}

int dprintf(int fd, const char* format, ...) {
	size_t	output_size;
	va_list	l_args;

	va_start(l_args, format);
	output_size = vdprintf(fd, format, l_args);
	va_end(l_args);
	return (output_size);
}

int	sprintf(char* buffer, const char *format, ...) {
	size_t	output_size;
	va_list	l_args;

	va_start(l_args, format);
	output_size = vsprintf(buffer, format, l_args);
	va_end(l_args);
	return (output_size);
}

int	printf(const char *format, ...) {
	int		output_size;
	va_list	l_args;

	va_start(l_args, format);
	output_size = vdprintf(STD_OUT, format, l_args);
	va_end(l_args);
	return (output_size);
}
