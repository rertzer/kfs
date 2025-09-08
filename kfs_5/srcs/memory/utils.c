#include <kernel.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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


