#include "keycode.h"
#include "scheduler.h"
#include "unistd.h"

int pause() {
	scheduler_set_current_status(PROC_SLEEP);
	scheduler_switch_status();
	scheduler_switch_task(true);
	return (scheduler_get_current_exit_status());
}
