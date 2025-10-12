#ifndef FORK_H
#define FORK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tss.h"

typedef struct {
	uint32_t old_ebp;
	uint32_t old_esp;
	uint32_t ret_address;
} fork_data_t;

int16_t fork();

#endif
