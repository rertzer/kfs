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

bool is_alnum(uint8_t c) {
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
		return (true);
	} else {
		return (false);
	}
}

void ft_memset(uint8_t* address, char c, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i) {
		address[i] = c;
	}
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
