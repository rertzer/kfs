#ifndef KFS_CTYPE_H
#define KFS_CTYPE_H

#include <stdbool.h>

#ifdef JROS
#define kfs_isspace isspace
#define kfs_isdigit isdigit
#define kfs_isupper isupper
#define kfs_islower islower
#endif

inline int kfs_isspace(int c) {
	return (c == ' ' || (c >= 9 && c <= 13));
}

inline int kfs_isdigit(int c) {
	return (c >= '0' && c <= '9');
}

inline int kfs_isupper(int c) {
	return (c >= 'A' && c <= 'Z');
}

inline int kfs_islower(int c) {
	return (c >= 'a' && c <= 'z');
}

#endif
