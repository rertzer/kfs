#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include "libkfs.h"

inline size_t strlen(char const* const str);
inline char*  strrchr(const char* str, char c);
inline int	  memcmp(const void* s1, const void* s2, size_t n);
inline void*  memset(void* address, char c, size_t size);

#ifndef JROS
int	  strncmp(const char* s1, const char* s2, size_t n);
char* strcpy(char* dest, const char* src);
#endif
#endif
