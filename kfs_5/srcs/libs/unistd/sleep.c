#include "kernel.h"
#include "scheduler.h"
#include "unistd.h"

uint32_t sleep(uint32_t time) {
	priority_t p;

	p.time = time * 1000 + get_timer_counter();
	p.proc = scheduler_get_current_proc();
	add_sleep(p);
	scheduler_set_current_status(PROC_SLEEP);
	scheduler_switch_status();
	sched_yield();
	uint32_t now = get_timer_counter();
	if (now >= p.time) {
		return (0);
	} else {
		return ((p.time - now) / 1000);
	}
}

uint8_t test_sleep(size_t argc, char** argv) {
	static uint32_t count = 10;
	(void)argc;
	(void)argv;
	while (true) {
		printf("having a nap for %d s\n", count);
		sleep(count);
		printf("woke up\n");
		++count;
		scheduler();
	}
	return (0);
}
