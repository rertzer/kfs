#include "utils.h"

// size_t strlen(const char* str) {
// 	size_t len = 0;
// 	while (str[len]) {
// 		++len;
// 	}
// 	return (len);
// }

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

int ft_memcmp(void* s1, void* s2, size_t n) {
	uint8_t* a = s1;
	uint8_t* b = s2;
	int		 diff = 0;
	for (size_t i = 0; diff == 0 && i < n; i++) {
		diff = a[i] - b[i];
	}
	return (diff);
}

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
