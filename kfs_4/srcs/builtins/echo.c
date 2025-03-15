#include "builtin.h"
#include "terminal.h"

extern volatile terminal_t term;

uint8_t echo(char* word, size_t word_len) {
	(void)word_len;
	printf("%s\n", word);
	return (0);
}
