#ifndef KFS_CTYPE_H
#define KFS_CTYPE_H

#include <stdbool.h>

#ifdef JROS
#define kfs_isspace isspace
#define kfs_isdigit isdigit
#define kfs_isupper isupper
#define kfs_islower islower
#define kfs_isalpha isalpha
#define kfs_isalnum isalnum
#define kfs_toupper toupper
#define kfs_tolower tolower
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

inline int kfs_isalpha(int c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

inline int kfs_isalnum(int c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

inline int kfs_toupper(int c) {
	return (c >= 'a' && c <= 'z') ? (c - 0x20) : c;
}

inline int kfs_tolower(int c) {
	return (c >= 'A' && c <= 'Z') ? (c + 0x20) : c;
}

#endif
