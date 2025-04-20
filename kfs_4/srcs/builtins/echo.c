#include "builtin.h"
#include "terminal.h"

extern volatile terminal_t term;

uint8_t echo(size_t argc, char** argv) {
	uint8_t ok = 0;
	if (argc > 1) {
		printf("%s\n", argv[1]);
	} else {
		ok = 1;
	}
	return (ok);
}
