#include <stddef.h>

#include "kfs_atoi.h"
#include "kfs_ctype.h"

int kfs_atoi(const char* str) {
	int	   nb = 0;
	int	   sign = 1;
	size_t i = 0;

	while (kfs_isspace(str[i])) {
		++i;
	}

	if (str[i] == '+') {
		++i;
	} else if (str[i] == '-') {
		sign = -1;
		++i;
	}

	for (; kfs_isdigit(str[i]); ++i) {
		nb = nb * 10 + (str[i] - '0');
	}
	return (sign * nb);
}
