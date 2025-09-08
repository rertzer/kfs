#include "builtin.h"
#include "memory.h"
#include "panic.h"
#include "utils.h"

uint8_t test_stack_dump(size_t argc, char** argv) {
	volatile char test[2048];
	if (argc == 2 && strncmp("panic", argv[1], 6) == 0) {
		for (int i = 0; i < 2048; ++i) {
			test[i] = 'B';
		}
		dump_stack();
		hexdump(0, NULL);
		panic(
			"Testing stack dump: "
			"Please run `readdump` after reboot\n");
		return (0);
	}
	readdump(0, NULL);
	return (test[0]);
}
