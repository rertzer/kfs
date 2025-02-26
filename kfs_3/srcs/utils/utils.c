#include <kernel.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) {
		++len;
	}
	return (len);
}

bool is_digit(uint8_t c) {
	return (c >= '0' && c <= '9');
}

bool is_upper(uint8_t c) {
	return (c >= 'A' && c <= 'Z');
}

bool is_lower(uint8_t c) {
	return (c >= 'a' && c <= 'z');
}

bool is_alpha(uint8_t c) {
	return (is_lower(c) || is_upper(c));
}

bool is_alnum(uint8_t c) {
	return (is_digit(c) || is_alpha(c));
}

int	ft_memcmp(void* s1, void* s2, size_t n) {
	uint8_t	*a = s1;
	uint8_t *b = s2;
	int	diff = 0;
	for (size_t i = 0; diff == 0 && i < n; i++) {
		diff = a[i] - b[i];
	}
	return (diff);
}

void ft_memset(uint8_t* address, char c, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i) {
		address[i] = c;
	}
}

void*	ft_memcpy(void* dest, const void* src, size_t n) {
	uint8_t	*d = dest;
	uint8_t	*s = (uint8_t*)src;
	for (uint32_t i = 0; i < n; ++i) {
		d[i] = s[i];
	}
	return (dest);
}

uint32_t round_up_power_two(uint32_t n) {
	uint32_t offset = 31;
	if (n == 0) {
		return (0);
	}
	for (; offset != 0; --offset) {
		if ((n >> offset) == 1) {
			break;
		}
	}
	if ((n - (1 << offset)) != 0) {
		++offset;
	}
	return (offset);
}

uint32_t index_len(uint32_t start, uint32_t end) {
	uint32_t len = (start <= end) ? end - start + 1 : 0;
	return (len);
}

uint32_t min(uint32_t a, uint32_t b) {
	return (a < b ? a : b);
}
