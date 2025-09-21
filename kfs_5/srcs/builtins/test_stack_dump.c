#include "builtin.h"
#include "panic.h"
#include "string.h"

uint8_t test_stack_dump(size_t argc, char** argv) {
	volatile char test[2048];
	int			  diff = kfs_strncmp("panic", argv[1], 6);

	if (argc == 2 && diff == 0) {
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
