#ifndef KFS_MEMCPY_H
#define KFS_MEMCPY_H

#include <stddef.h>
#include <stdint.h>

#ifdef JROS
#define kfs_memcpy memcpy
#endif

inline void* kfs_memcpy(void* dest, const void* src, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		((char*)dest)[i] = ((char*)src)[i];
	}
	return (dest);
}
#endif
