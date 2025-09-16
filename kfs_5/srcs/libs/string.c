#include "utils.h"

void ft_memset(void* address, char c, uint32_t size) {
	uint8_t* ptr = address;
	for (uint32_t i = 0; i < size; ++i) {
		ptr[i] = c;
	}
}

void* ft_memcpy(void* dest, const void* src, size_t n) {
	uint8_t* d = dest;
	uint8_t* s = (uint8_t*)src;
	for (uint32_t i = 0; i < n; ++i) {
		d[i] = s[i];
	}
	return (dest);
}

int strncmp(const char* s1, const char* s2, size_t n) {
	while ((*s1 || *s2) && n--) {
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return (0);
}
