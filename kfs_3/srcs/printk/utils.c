#include "printk.h"
#include "terminal.h"

size_t ft_atoi(const char* str) {
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

size_t nbr_len(int nbr) {
	size_t len = 0;

	if (nbr < 0) {
		nbr = -nbr;
		len += 1;
	}

	if (nbr > 9) {
		len += nbr_len(nbr / 10);
		len += 1;
	} else
		len += 1;
	return (len);
}

size_t ft_strrchr(const char* str, char c) {
	size_t i = 0;
	size_t res = 0;

	while (str[i]) {
		if (str[i] == c)
			res = i;
		++i;
	}
	return (res);
}

int get_hexa_len(unsigned int nbr) {
	int len = 0;
	if (nbr >= 16)
		len += get_hexa_len(nbr / 16);
	return (len + 1);
}
