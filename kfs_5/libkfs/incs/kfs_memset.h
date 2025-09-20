#ifndef KFS_MEMSET_H
#define KFS_MEMSET_H

#include <stddef.h>

inline void* kfs_memset(void* address, char c, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		((char*)address)[i] = c;
	}
	return (address);
}

#endif
