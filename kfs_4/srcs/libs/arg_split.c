#include "keycode.h"
#include "malloc.h"
#include "utils.h"

static void	  init_argc(arg_t* args, char const* const src);
static void	  init_argv(arg_t* args);
static size_t arg_count(char const* const src);
static void	  split_string(arg_t* args, char const* const str);
static size_t get_next_word_index(char const* const str, size_t i);
static size_t get_word_end_index(char const* const str, size_t i);
static bool	  test_line(char* line, size_t nb);

arg_t arg_split(char const* const src) {
	arg_t args;

	init_argc(&args, src);
	init_argv(&args);

	if (args.argc != 0) {
		split_string(&args, src);
	}

	return (args);
}

static void init_argc(arg_t* args, char const* const src) {
	if (src != NULL) {
		args->argc = arg_count(src);
	} else {
		args->argc = 0;
	}
}

static size_t arg_count(const char* const src) {
	size_t arg_nb = 0;
	bool   is_word = false;

	for (size_t i = 0; src[i] != '\0'; ++i) {
		if (is_word == true && (src[i] == ' ' || src[i] == '\t')) {
			is_word = false;
		} else if (is_word == false && src[i] != ' ' && src[i] != '\t') {
			is_word = true;
			++arg_nb;
		}
	}
	return (arg_nb);
}

static void init_argv(arg_t* args) {
	if (args->argc == 0) {
		args->argv = NULL;
		return;
	}

	args->argv = kpmalloc(sizeof(char*) * args->argc + 1);
	if (args->argv == NULL) {
		args->argc = 0;
	} else {
		args->argv[args->argc] = NULL;
	}
}

static void split_string(arg_t* args, char const* const src) {
	size_t start;
	size_t end = 0;

	for (size_t i = 0; src[i] != '\0'; ++i) {
		start = get_next_word_index(src, end);
		if (src[start] == '\0') {
			break;
		}
		end = get_word_end_index(src, start);
		args->argv[i] = jr_substr(src, start, end - start);
		if (args->argv[i] == NULL) {
			arg_split_clean(args);
			break;
		}
	}
}

static size_t get_next_word_index(char const* const str, size_t i) {
	for (; str[i] == ' ' || str[i] == '\t'; ++i)
		;
	return (i);
}

static size_t get_word_end_index(char const* const str, size_t i) {
	for (; str[i] != ' ' && str[i] != '\t' && str[i] != '\0'; ++i)
		;
	return (i);
}

void arg_split_clean(arg_t* args) {
	if (args->argv == NULL) {
		args->argc = 0;
		return;
	}
	for (size_t i = 0; args->argv[i] != 0; ++i) {
		kpfree(args->argv[i]);
	}
	kpfree(args->argv);
	args->argc = 0;
	args->argv = NULL;
}

/* tests */
bool arg_split_test() {
	if (test_line(NULL, 0) == false) {
		return (false);
	}
	if (test_line("", 0) == false) {
		return (false);
	}

	if (test_line("ma ligne de test 	compte    sept arguments", 7) == false) {
		return (false);
	}

	if (test_line("             trim			   ", 1) == false) {
		return (false);
	}
	printf("arg_split test succeded\n");
	return (true);
}

static bool test_line(char* line, size_t nb) {
	char  ok = false;
	arg_t arg = arg_split(line);
	printf("original: \"%s\"\nnombre d'arguments: %d\n", line, arg.argc);
	for (size_t i = 0; i < arg.argc; ++i) {
		printf("%d\t\"%s\"\n", i, arg.argv[i]);
	}
	if (arg.argc == nb) {
		ok = true;
	}
	arg_split_clean(&arg);
	return (ok);
}
