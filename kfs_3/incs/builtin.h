#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>

#define REGS_NB 15

typedef enum {
	ECHO,
	QWERTY,
	AZERTY,
	KREBOOT,
	REBOOT,
	HALT,
	REGISTERS,
	HEXDUMP,
	INT,
	CLEAN,
	BUILTINS_NB
} builtin_t;

#define BUILTINS                                                                      \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [KREBOOT] = "kreboot", \
	[REBOOT] = "reboot", [HALT] = "halt", [REGISTERS] = "registers", [HEXDUMP] = "hexdump", \
	[INT] = "int", [CLEAN] = "clean"

uint8_t echo(char* word, size_t word_len);
uint8_t qwerty();
uint8_t azerty();
uint8_t kreboot();
uint8_t reboot();
uint8_t halt();
uint8_t registers();
void	freboot();
uint8_t trigger_interrupt();

#endif
