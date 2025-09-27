#include "builtin.h"
#include "interrupts.h"
#include "stdlib.h"

extern void (*int_builtin_table[])(void);

uint8_t interrupt(size_t argc, char** argv) {
	uint8_t ret = 0;
	if (argc > 1) {
		uint8_t int_nb = atoi(argv[1]);
		if (int_nb < IDT_MAX_DESCRIPTORS) {
			int_builtin_table[int_nb]();
		} else {
			printf("Invalid Interrupt number: %u\n", int_nb);
			ret = 1;
		}
	} else {
		printf("interrupt require an argument\n");
		ret = 1;
	}

	return (ret);
}
