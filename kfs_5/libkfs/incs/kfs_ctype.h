#ifndef KFS_CTYPE_H
#define KFS_CTYPE_H

#include <stdbool.h>

#ifdef JROS
#define kfs_isspace isspace
#endif

inline int kfs_isspace(int c) {
	if (c == ' ' || (c >= 9 && c <= 13)) {
		return (true);
	}
	return (false);
}

#endif
