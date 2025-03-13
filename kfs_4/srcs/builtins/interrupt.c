#include "builtin.h"
#include "interrupts.h"
#include "utils.h"

extern void (*int_builtin_table[])(void);

uint8_t interrupt(char* pointer, size_t len) {
	uint8_t ret = 0;
	(void)len;
	uint8_t int_nb = ft_atoi(pointer);
	if (int_nb < IDT_MAX_DESCRIPTORS) {
		int_builtin_table[int_nb]();
	} else {
		printf("Invalid Interrupt number: %u\n", int_nb);
		ret = 1;
	}
	return (ret);
}
