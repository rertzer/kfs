#ifndef LIBKFS_H
#define LIBKFS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef JROS
#define kfs_strlen strlen
#define kfs_strrchr strrchr
#define kfs_memcmp memcmp
#define kfs_memset memset
#endif

/* ============================ functions ================================== */

#include "kfs_memcmp.h"
#include "kfs_memset.h"
#include "kfs_strlen.h"
#include "kfs_strrchr.h"
#endif
