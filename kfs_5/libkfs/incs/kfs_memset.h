#ifndef KFS_MEMSET_H
#define KFS_MEMSET_H

#include <stddef.h>

inline void* kfs_memset(void* address, char c, size_t size) {
	(void)c;
	(void)size;
	return (address);
}

#endif
