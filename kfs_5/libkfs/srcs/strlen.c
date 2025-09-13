#include "kfs_strlen.h"

size_t kfs_strlen(char* str) {
	size_t len = 0;

	while (str[len] != '\0') {
		++len;
	}
	return (len);
}
