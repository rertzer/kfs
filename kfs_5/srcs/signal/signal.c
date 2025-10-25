#include "signal.h"
#include <stddef.h>
#include "scheduler.h"
#include "stdio.h"
#include "utils_inline.h"

static int get_pending_signal(proc_t* current);

void pending_signals(proc_t* current) {
	int sig = get_pending_signal(current);

	while (sig) {
		sig_handler_t handler = get_signal_default_handler(sig);
		if (handler != NULL) {
			current->sig_processing = set_bit(current->sig_processing, sig);
			handler();
			current->sig_processing = unset_bit(current->sig_processing, sig);
		}
		current->sig_pending = unset_bit(current->sig_processing, sig);
		sig = get_pending_signal(current);
	}
}

static int get_pending_signal(proc_t* current) {
	int sig = 0;

	for (size_t i = 0; i < SIG_LIMIT; ++i) {
		if (get_bit(current->sig_pending, i) && !get_bit(current->sig_processing, i)) {
			sig = i;
			break;
		}
	}

	return (sig);
}

sig_handler_t get_signal_default_handler(signal_t sig) {
	static const sig_default_t default_value[SIG_LIMIT] = {SIG_DEFAULT_VALUES};
	static const sig_handler_t sig_default_handler[5] = {NULL, sig_exit, sig_core, sig_stop, sig_continue};

	return (sig_default_handler[default_value[sig]]);
}

void sig_exit() {
	printf("sig exit\n");
}
void sig_core() {
	printf("sig core\n");
}
void sig_continue() {
	printf("sig continue\n");
}
void sig_stop() {
	printf("sig stop\n");
};

void sig_ignore() {
	printf("sig igonre\n");
}

void kill(uint16_t pid, signal_t sig) {
	proc_t* current = scheduler_get_current_proc();
	proc_t* target = scheduler_get_proc_by_pid(pid);
	if (current->owner == 0 || current->owner == target->owner) {
		target->sig_pending = set_bit(target->sig_pending, sig);
	}
}
