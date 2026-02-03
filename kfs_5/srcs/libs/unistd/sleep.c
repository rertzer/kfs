#include "builtin.h"
#include "kernel.h"
#include "keycode.h"
#include "scheduler.h"
#include "unistd.h"

uint32_t sleep(uint32_t time) {
	priority_t p;

	p.time = time * 1000 + get_timer_counter();
	printk("time %d ptime %d counter %d\n", time, p.time, get_timer_counter());
	p.proc = scheduler_get_current_proc();
	add_sleep(p);
	scheduler_set_current_status(PROC_SLEEP);
	scheduler_switch_status();
	printk("good night\n");
	ps(42, NULL);
	scheduler_switch_task(true);
	printk("good morning\n");
	ps(42, NULL);
	uint32_t now = get_timer_counter();
	if (now >= p.time) {
		return (0);
	} else {
		return ((p.time - now) / 1000);
	}
}

uint8_t test_sleep(size_t argc, char** argv) {
	static uint32_t count = 5;
	(void)argc;
	(void)argv;
	while (count) {
		printf("having a nap for %d s\n", count);
		sleep(count);
		printf("woke up\n");
		--count;
		ps(42, NULL);
		scheduler();
	}
	return (0);
}
