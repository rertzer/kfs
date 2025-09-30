#ifndef KFS_INT_MATH_H
#define KFS_INT_MATH_H

#include <stdint.h>

#ifdef JROS
#define kfs_min min
#define kfs_max max
#define kfs_abs abs
#endif

inline int32_t kfs_min(int32_t a, int32_t b) {
	return (a < b ? a : b);
}

inline int32_t kfs_max(int32_t a, int32_t b) {
	return (a > b ? a : b);
}

inline uint32_t kfs_abs(int32_t v) {
	return (v >= 0 ? v : -v);
}

#endif
