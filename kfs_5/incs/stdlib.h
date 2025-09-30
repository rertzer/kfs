#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>
#include "libkfs.h"

int	  atoi(const char* str);
char* itoa(int val, char* buffer, int radix);

#ifndef JROS
void* malloc(size_t size);
void  free(void* ptr);
#endif

#endif	// STDLIB_H
