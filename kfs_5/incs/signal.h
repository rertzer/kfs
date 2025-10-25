#ifndef SIGNAL_H
#define SIGNAL_H

#include "processus.h"

#define SIG_LIMIT 32

typedef enum {
	SIGHUP = 1,
	SIGINT,
	SIQUIT,
	SIGILL,
	SIGTRAP,
	SIGABRT,
	SIGIOT = 6,
	SIGBUS,
	SIGFPE,
	SIGKILL,
	SIGUSR1,
	SIGSEGV,
	SIGUSR2,
	SIGPIPE,
	SIGALARM,
	SIGTERM,
	SIGSTKFLT,
	SIGCHLD,
	SIGCONT,
	SIGSTOP,
	SIGTSTP,
	SIGTTIN,
	SIGTTOU,
	SIGURG,
	SIGXCPU,
	SIGXFSZ,
	SIGVTALARM,
	SIGPROF,
	SIGWINCH,
	SIGIO,
	SIGPOLL = 29,
	SIGPWR,
	SIGSYS,
	SIGUNUSED = 31
} signal_t;

typedef enum { SIG_IGNORE, SIG_EXIT, SIG_CORE, SIG_STOP, SIG_CONTINUE } sig_default_t;

#define SIG_DEFAULT_VALUES                                                                                            \
	[SIGHUP] = SIG_EXIT, [SIGINT] = SIG_EXIT, [SIQUIT] = SIG_CORE, [SIGILL] = SIG_CORE, [SIGTRAP] = SIG_CORE,         \
	[SIGABRT] = SIG_CORE, [SIGBUS] = SIG_CORE, [SIGFPE] = SIG_CORE, [SIGKILL] = SIG_EXIT, [SIGUSR1] = SIG_EXIT,       \
	[SIGSEGV] = SIG_CORE, [SIGUSR2] = SIG_EXIT, [SIGPIPE] = SIG_EXIT, [SIGALARM] = SIG_EXIT, [SIGTERM] = SIG_EXIT,    \
	[SIGSTKFLT] = SIG_EXIT, [SIGCHLD] = SIG_IGNORE, [SIGCONT] = SIG_CONTINUE, [SIGSTOP] = SIG_STOP,                   \
	[SIGTSTP] = SIG_STOP, [SIGTTIN] = SIG_STOP, [SIGTTOU] = SIG_STOP, [SIGURG] = SIG_IGNORE, [SIGXCPU] = SIG_CORE,    \
	[SIGXFSZ] = SIG_CORE, [SIGVTALARM] = SIG_EXIT, [SIGPROF] = SIG_EXIT, [SIGWINCH] = SIG_IGNORE, [SIGIO] = SIG_EXIT, \
	[SIGPWR] = SIG_EXIT, [SIGSYS] = SIG_CORE,

typedef void (*sig_handler_t)(void);

void		  sig_exit();
void		  sig_core();
void		  sig_continue();
void		  sig_stop();
sig_handler_t get_signal_default_handler(signal_t sig);
void		  pending_signals(proc_t* current);
void		  kill(uint16_t pid, signal_t sig);

#endif
