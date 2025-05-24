#include "builtin.h"
#include "kernel.h"
#include "time.h"

static void set_time(time_t* time, uint32_t ms);
static void print_time(time_t* time);

uint8_t uptime(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	time_t time;

	set_time(&time, get_pit_total_ms());
	print_time(&time);

	return (0);
}

static void set_time(time_t* time, uint32_t ms) {
	printf("ms %u mid %u dy %u\n", ms, MS_IN_DAY, ms / MS_IN_DAY);
	time->days = ms / MS_IN_DAY;
	ms -= time->days * MS_IN_DAY;
	time->hours = ms / MS_IN_HOUR;
	ms -= time->hours * MS_IN_HOUR;
	time->minutes = ms / MS_IN_MIN;
	ms -= time->minutes * MS_IN_MIN;
	time->seconds = ms / MS_IN_SEC;
	ms -= time->seconds * MS_IN_SEC;
	time->milliseconds = ms;
}

static void print_time(time_t* time) {
	printf("%u days %u hours %u minutes %u seconds %u milliseconds elapsed since boot\n", time->days, time->hours,
		   time->minutes, time->seconds, time->milliseconds);
}
