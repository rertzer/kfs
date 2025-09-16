#ifndef KFS_MEMCMP
#define KFS_MEMCMP

#include <stddef.h>
#include <stdint.h>

inline int kfs_memcmp(const void* s1, const void* s2, size_t n) {
	unsigned char* str1 = (unsigned char*)s1;
	unsigned char* str2 = (unsigned char*)s2;
	int			   diff = 0;

	for (size_t i = 0; diff == 0 && i < n; i++) {
		diff = str1[i] - str2[i];
	}
	return (diff);
}
#endif
