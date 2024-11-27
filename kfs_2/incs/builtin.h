#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>

#define REGS_NB 14

typedef enum {
	ECHO,
	QWERTY,
	AZERTY,
	KREBOOT,
	REBOOT,
	HALT,
	REGISTERS,
	HEXDUMP,
	BUILTINS_NB
} builtin_t;

#define BUILTINS                                                                      \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [KREBOOT] = "kreboot", \
	[REBOOT] = "reboot", [HALT] = "halt", [REGISTERS] = "registers", [HEXDUMP] = "hexdump"

uint8_t echo(char* word, size_t word_len);
uint8_t qwerty();
uint8_t azerty();
uint8_t kreboot();
uint8_t reboot();
uint8_t halt();
uint8_t registers();
void	freboot();

#endif
