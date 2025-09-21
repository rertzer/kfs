#include "ctype.h"
#include "utils.h"

size_t ft_atoi(const char* str) {
	size_t i = 0;
	size_t res = 0;
	size_t sign = 1;

	while (isspace(str[i]))
		++i;
	if (str[i] == '-' || str[i] == '+') {
		++i;
		if (str[i - 1] == '-')
			sign = -1;
	}
	while (is_digit(str[i])) {
		res = res * 10 + str[i] - '0';
		++i;
	}
	return (res * sign);
}
