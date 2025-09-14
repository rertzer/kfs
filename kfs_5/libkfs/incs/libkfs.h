#ifndef LIBKFS_H
#define LIBKFS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef JROS
#define kfs_strlen strlen
size_t strlen(const char* const str);
#endif

/* ============================ functions ================================== */

size_t kfs_strlen(const char* const str);

#endif
