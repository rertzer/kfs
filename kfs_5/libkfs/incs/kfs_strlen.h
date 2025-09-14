#ifndef KFS_STRLEN_H
#define KFS_STRLEN_H

#include "stddef.h"

inline size_t kfs_strlen(const char* const str) {
	size_t len = 0;

	while (str[len] != '\0') {
		++len;
	}
	return (len);
}

#endif
