#ifndef KFS_STRNCMP_H
#define KFS_STRNCMP_H

#include <stdint.h>
#include "stddef.h"

#ifdef JROS
#define kfs_strncmp strncmp
#endif

#ifdef JROS
#define kfs_strlen strlen
#define kfs_strncmp strncmp
#define kfs_strrchr strrchr
#define kfs_memcmp memcmp
#define kfs_memset memset
#define kfs_memcpy memcpy
#endif

int kfs_strncmp(const char* s1, const char* s2, size_t n);
#endif
