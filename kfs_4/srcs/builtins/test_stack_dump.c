#include "builtin.h"
#include "utils.h"
#include "panic.h"
#include "memory.h"
#include "builtin.h"

uint8_t test_stack_dump(size_t argc, char** argv) {
	if (argc == 2 && strncmp("panic", argv[1], 6) == 0) {
        volatile char test[2048];
	    for (int i = 0; i < 2048; ++i) {
	    	test[i] = 'B';
	    }
        dump_stack();
        hexdump(0, NULL);
        panic(
            "Testing stack dump: "
            "Please run `hexdump dump` after reboot\n");
        return (0);
    }
    readdump(0, NULL);
    return (0);
}