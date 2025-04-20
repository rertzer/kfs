#include "builtin.h"
#include "kernel.h"
#include "malloc.h"
#include "terminal.h"
#include "utils.h"

extern volatile terminal_t term;

static uint8_t line_exec(arg_t* args);
static uint8_t cmd_cmp(char const* const cmd, const char* const builtin);
static uint8_t cmd_error(char const* const cmd);

bool process_line() {
	char* line = readline();
	if (line == NULL) {
		return (false);
	}
	arg_t args = arg_split(line);
	if (args.argc != 0) {
		line_exec(&args);
	}
	arg_split_clean(&args);
	kpfree(line);
	return (true);
}

char* readline() {
	size_t len = term_line_len();
	if (len == 0 || len > 2000) {
		return (NULL);
	}
	char* line = term_substr(term.buffer, term_prompt_pos(), len);
	return (line);
}

static uint8_t line_exec(arg_t* args) {
	static const char*		   builtins[BUILTINS_NB] = {BUILTINS};
	static const builtin_fun_t builtins_fun[BUILTINS_NB] = {BUILTIN_FUNCTIONS};

	uint8_t ret = 0;
	bool	found = false;

	for (size_t i = 0; i < BUILTINS_NB; ++i) {
		if (cmd_cmp(args->argv[0], builtins[i]) == 0) {
			char*  arg_one = NULL;
			size_t arg_one_len = 0;
			if (args->argc > 1) {
				arg_one = args->argv[1];
				arg_one_len = strlen(arg_one);
			}
			ret = builtins_fun[i](arg_one, arg_one_len);
			found = true;
			break;
		}
	}
	if (found == false) {
		ret = cmd_error(args->argv[0]);
	}
	return (ret);
}

static uint8_t cmd_cmp(char const* const cmd, const char* const builtin) {
	size_t i = 0;
	for (; cmd[i] != '\0' && builtin[i] != '\0'; ++i) {
		if (cmd[i] != builtin[i]) {
			break;
		}
	}
	return (cmd[i] - builtin[i]);
}

static uint8_t cmd_error(char const* const cmd) {
	printk("Invalid command: %s\n", cmd);
	return (1);
}
