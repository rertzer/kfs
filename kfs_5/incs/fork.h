#ifndef FORK_H
#define FORK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tss.h"

#define ERROR_FORK_NO_PARENT -1
#define ERROR_FORK_SPAWN_FAILED -2
#define ERROR_FORK_ADD_GDT_FAILED -3

int16_t fork();

#endif
