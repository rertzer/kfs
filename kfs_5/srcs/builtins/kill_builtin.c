#include "builtin.h"
#include "signal.h"
#include "stdlib.h"

extern void (*int_builtin_table[])(void);

uint8_t kill_builtin(size_t argc, char** argv) {
	uint8_t ret = 0;
	if (argc > 2) {
		uint16_t pid = atoi(argv[1]);
		signal_t sig = atoi(argv[2]);
		if (sig >= SIG_LIMIT) {
			printf("unknown signal value %d\n", sig);
		} else {
			kill(pid, sig);
		}

	} else {
		printf("kill requires two arguments\n");
		ret = 1;
	}

	return (ret);
}
