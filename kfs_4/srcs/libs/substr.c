#include "malloc.h"

char* jr_substr(char const* const src, size_t start, size_t len) {
	char* dest = kpmalloc(sizeof(char) * len + 1);
	if (dest == NULL) {
		return (NULL);
	}
	for (size_t i = 0; i < len; ++i) {
		dest[i] = src[start + i];
	}
	dest[len] = '\0';
	return (dest);
}

char* term_substr(uint16_t const* const src, size_t start, size_t len) {
	char* dest = kpmalloc(sizeof(char) * len + 1);
	if (dest == NULL) {
		return (NULL);
	}
	for (size_t i = 0; i < len; ++i) {
		dest[i] = (char)src[start + i];
	}
	dest[len] = '\0';
	return (dest);
}
