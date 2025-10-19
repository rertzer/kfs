#include "signal.h"

sig_handler_t get_signal_default_handler(signal_t sig) {
	static const sig_handler_t default_handler[46] = {SIG_DEFAULT_VALUES};

	return (default_handler[sig]);
}
