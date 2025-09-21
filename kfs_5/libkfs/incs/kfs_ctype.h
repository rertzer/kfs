#ifndef KFS_CTYPE_H
#define KFS_CTYPE_H

#include <stdbool.h>

#ifdef JROS
#define kfs_isspace isspace
#define kfs_isdigit isdigit
#endif

inline int kfs_isspace(int c) {
	return (c == ' ' || (c >= 9 && c <= 13));
}

inline int kfs_isdigit(int c) {
	return (c >= '0' && c <= '9');
}
#endif
