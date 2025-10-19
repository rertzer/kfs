#ifndef SIGNAL_H
#define SIGNAL_H

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
	SIGWSYS,
	SIGUNUSED = 31
} signal_t;

#define SIG_DEFAULT_VALUES                                                                                           \
[SIGHUP] = sig_exit, [SIGINT] = sig_exit, [SIQUIT] = sig_core, [SIGILL] = sig_core, [SIGTRAP] = sig_core, \
[SIGABRT] = sig_core,  [SIGBUS] = sig_core, [SIGFPE] = sig_core, [SIGKILL] = sig_exit,  [SIGUSR1] = sig_exit, \
[SIGSEGV] = sig_core, [SIGUSR2] = sig_exit, [SIGPIPE] = sig_exit, [SIGALARM] = sig_exit, [SIGTERM] = sig_exit, \
[SIGSTKFLT] = sig_exit, [SIGCHLD] = sig_ignore, [SIGCONT = sig_continue, [SIGSTOP] = sig_stop, [SIGTSTP] = sig_stop, \
[SIGTTIN] = sig_stop, [SIGTTOU] = sig_stop, [SIGURG] = sig_ignore,  [SIGXCPU] = sig_core, [SIGXFSZ] = sig_core, \
[SIGVTALARM] = sig_exit, [SIGPROF] = sig_exit, [SIGWINCH]= sig_ignore [SIGIO] = sig_exit, [SIGPWR] = sig_exit, \
[SIGSYS] = sig_core

typedef void (*sig_handler_t)(void);

void		  sig_exit();
void		  sig_core();
void		  sig_continue();
void		  sig_stop();
void		  sig_ignore();
sig_handler_t get_signal_default_handler(signal_t sig);

#endif
