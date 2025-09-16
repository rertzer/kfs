#ifndef KFS_STRRCHR_H
#define KFS_STRRCHR_H

#include <stdint.h>
#include "stddef.h"

inline char* kfs_strrchr(const char* str, char c) {
	size_t i = 0;
	char*  found = NULL;
	while (str[i] != '\0') {
		if (str[i] == c) {
			found = (char*)str + i;
		}
		++i;
	}
	if (str[i] == c) {
		found = (char*)str + i;
	}
	return (found);
}

#endif
