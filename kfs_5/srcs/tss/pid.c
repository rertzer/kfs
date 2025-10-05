#include "kfs_bitmap.h"
#include "printk.h"
#include "processus.h"

size_t pid_bitmap[PID_BITMAP_SIZE];
size_t last_pid;
size_t active_pid_nb;

void pid_bitmap_init() {
	bitmap_erase(pid_bitmap, MAX_PID);
	set_bitmap_value(pid_bitmap, 0, 1);
	last_pid = 0;
	active_pid_nb = 1;
}

int16_t pid_bitmap_get_new() {
	if (active_pid_nb == MAX_PROC_NB) {
		return (0);
	}
	printf("searching %d\t", last_pid);
	uint32_t new_pid = get_next_bitmap(pid_bitmap, MAX_PID, last_pid);
	if (new_pid == 0) {
	}
	if (new_pid == last_pid) {
		new_pid = 0;
	} else {
		set_bitmap_value(pid_bitmap, new_pid, 1);
		last_pid = new_pid;
		++active_pid_nb;
	}
	return (new_pid);
}

void pid_bitmap_remove(int16_t pid) {
	set_bitmap_value(pid_bitmap, pid, 0);
	--active_pid_nb;
}

/* ============================= TEST =================================*/

void pid_bitmap_test() {
	pid_bitmap_init();
	printf("pid bitmap testing\n");
	for (size_t i = 0; i < MAX_PID; ++i) {
		if (get_bitmap_value(pid_bitmap, i) != 0) {
			printf("PID_BITMAP_TEST ERROR\n");
			break;
		}
	}
	printf("testing get new pid\n");
	for (size_t i = 0; i < (MAX_PROC_NB - 1); ++i) {
		uint32_t pid = pid_bitmap_get_new();

		if (pid != last_pid) {
			printf("PID_BITMAP_TEST ERROR new pid: %d last pid: %d\n", pid, last_pid);
			break;
		}
	}
	uint32_t pid = pid_bitmap_get_new();
	if (pid != 0 || active_pid_nb != 1024) {
		printf("PID TEST ERROR: to many active pids %d %d\n", pid, active_pid_nb);
	}
	printf("last pid is now: %d\nremoving some...\n", last_pid);
	pid_bitmap_remove(42);
	if (active_pid_nb != 1023) {
		printf("pid bitmap test error: remove error\n");
	}
	pid = pid_bitmap_get_new();
	if (pid != 1024) {
		printf("pid bitmap error\n");
	}

	for (size_t i = 0; i < SIZE_MAX; ++i) {
		pid_bitmap_remove(last_pid);
		uint32_t pid = pid_bitmap_get_new();
		// if ((i % 1024) == 0) {
		printf("processing  %d %d\n", pid, last_pid);
		// }
		if (pid == 0) {
			printf("ERROR  %d %d\n", pid, last_pid);
			break;
		}
	}
}
