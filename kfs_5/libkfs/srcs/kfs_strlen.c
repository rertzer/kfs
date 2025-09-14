#include "libkfs.h"

size_t kfs_strlen(const char* const str) {
	size_t len = 0;

	while (str[len] != '\0') {
		++len;
	}
	return (len);
}
