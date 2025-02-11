#include "builtin.h"
#include "terminal.h"

extern volatile terminal_t term;

uint8_t echo(char* word, size_t word_len) {
	for (size_t i = 0; i < word_len; ++i) {
		term_putchar(word[i]);
	}
	term_putchar('\n');
	return (0);
}
