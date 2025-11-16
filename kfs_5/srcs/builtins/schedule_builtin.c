#include "builtin.h"
#include "scheduler.h"

uint8_t schedule_builtin(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	scheduler();

	return (0);
}
