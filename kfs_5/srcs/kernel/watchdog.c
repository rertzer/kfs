#include "kernel.h"
#include "scheduler.h"
#include "wait.h"

uint8_t watchdog(size_t argc, char** argv) {
	(void)argc;
	(void)argv;

	while (true) {
		waitpid(0, NULL, WNOHANG);
		sched_yield();
	}
}
