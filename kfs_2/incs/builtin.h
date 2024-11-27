#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
	ECHO,
	QWERTY,
	AZERTY,
	KREBOOT,
	REBOOT,
	HALT,
	QUIT,
	DIVIDE,
	INT,
	HELP,
	TEST,
	BUILTINS_NB
} builtin_t;

#define BUILTINS                                                                               \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [KREBOOT] = "kreboot",          \
	[REBOOT] = "reboot", [HALT] = "halt", [QUIT] = "quit", [DIVIDE] = "divide", [INT] = "int", \
	[HELP] = "help", [TEST] = "test"

uint8_t echo(char* word, size_t word_len);
uint8_t qwerty();
uint8_t azerty();
uint8_t kreboot();
uint8_t reboot();
uint8_t test();
uint8_t halt();
void	freboot();

#endif
