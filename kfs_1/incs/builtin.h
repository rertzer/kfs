#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>


#define BUILTINS                                                                \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [QUIT] = "quit", \
	[DIVIDE] = "divide", [INT] = "int", [HELP] = "help"

uint8_t echo(char* word, size_t word_len);
uint8_t qwerty();
uint8_t azerty();
#endif
