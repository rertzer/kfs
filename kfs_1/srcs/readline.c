#include "builtin.h"
#include "kernel.h"

extern volatile terminal_t term;

static void	   cmdline_init(cmdline_t* cmdline);
static void	   cmdline_set(cmdline_t* cmdline);
static size_t  cmdline_word(size_t pos, bool inside);
static uint8_t cmdline_exec(cmdline_t* cmdline);
static uint8_t cmd_cmp(cmdline_t* cmdline, const char* builtin);
static uint8_t cmd_error(cmdline_t* cmdline);

bool readline() {
	cmdline_t cmdline;
	cmdline_init(&cmdline);
	cmdline_set(&cmdline);

	cmdline_exec(&cmdline);
	return (true);
}

static void cmdline_init(cmdline_t* cmdline) {
	for (size_t i = 0; i < 256; ++i) {
		cmdline->cmd[i] = '\0';
		cmdline->arg[i] = '\0';
	}
	cmdline->cmd_len = 0;
	cmdline->arg_len = 0;
}

static void cmdline_set(cmdline_t* cmdline) {
	size_t start = cmdline_word(term.prompt_row * VGA_WIDTH + term.prompt_column, false);
	size_t end = cmdline_word(start, true);
	cmdline->cmd_len = end - start;
	if (cmdline->cmd_len > 255) {
		cmdline->cmd_len = 255;
	}
	for (size_t i = 0; i < cmdline->cmd_len; ++i) {
		cmdline->cmd[i] = term.buffer[start + i];
	}
	cmdline->cmd[cmdline->cmd_len] = '\0';

	start = cmdline_word(end, false);
	end = cmdline_word(start, true);
	cmdline->arg_len = end - start;
	if (cmdline->arg_len > 255) {
		cmdline->arg_len = 255;
	}
	for (size_t i = 0; i < cmdline->arg_len; ++i) {
		cmdline->arg[i] = term.buffer[start + i];
	}
	cmdline->arg[cmdline->arg_len] = '\0';
}

static size_t cmdline_word(size_t pos, bool inside) {
	while (pos < term.row * VGA_WIDTH + term.column) {
		if (is_alnum(term.buffer[pos]) == inside) {
			++pos;
		} else {
			break;
		}
	}
	return (pos);
}

static uint8_t cmdline_exec(cmdline_t* cmdline) {
	static const char* builtins[BUILTINS_NB] = {BUILTINS};

	uint8_t ret = 0;
	bool	found = false;

	for (size_t i = 0; i < BUILTINS_NB; ++i) {
		if (cmd_cmp(cmdline, builtins[i]) == 0) {
			switch (i) {
				case ECHO:
					ret = echo(cmdline->arg, cmdline->arg_len);
					found = true;
					break;
				case AZERTY:
					ret = azerty();
					found = true;
					break;
				case QWERTY:
					ret = qwerty();
					found = true;
					break;
				case TEST:
					ret = test();
					found = true;
					break;
				default:
					break;
			}
		}
		if (found == true) {
			break;
		}
	}
	if (found == false) {
		ret = cmd_error(cmdline);
	}
	return (ret);
	// for (size_t i = 0; i < BUILTINS_NB; ++i) {
	// 	printk("%d : %s\n", i, builtins[i]);
	// }
}

static uint8_t cmd_cmp(cmdline_t* cmdline, const char* builtin) {
	size_t i = 0;
	for (; i < cmdline->cmd_len && builtin[i] != '\0'; ++i) {
		if (cmdline->cmd[i] != builtin[i]) {
			break;
		}
	}
	return (cmdline->cmd[i] - builtin[i]);
}

static uint8_t cmd_error(cmdline_t* cmdline) {
	printk("Invalid command: ");
	for (size_t i = 0; i < cmdline->cmd_len; ++i) {
		term_putchar(cmdline->cmd[i]);
	}
	term_putchar('\n');
	return (1);
}
