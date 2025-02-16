#include "builtin.h"
#include "terminal.h"

extern volatile terminal_t term;

uint8_t echo(char* word, size_t word_len) {
	term_putchars_ln(word, word_len);
	return (0);
}
