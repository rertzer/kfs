#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include "libkfs_jros.h"

inline size_t strlen(char const* const str);
inline char*  strrchr(const char* str, char c);
inline int	  memcmp(const void* s1, const void* s2, size_t n);
inline void*  memcpy(void* dest, const void* src, size_t n);
inline void*  memset(void* address, char c, size_t size);
int			  strncmp(const char* s1, const char* s2, size_t n);

#ifndef JROS
int	  strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
#endif
#endif
