#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

char* itoa(unsigned long val, char* buffer, unsigned long radix);

#ifndef JROS
void* malloc(size_t size);
void  free(void* ptr);
#endif

#endif	// STDLIB_H
