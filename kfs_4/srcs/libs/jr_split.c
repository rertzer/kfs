#include "malloc.h"
#include "utils.h"

static size_t init_args(arg_t* args, char const* const src);
static size_t init_argv(arg_t* args);
static size_t arg_count(arg_t* args, char const* const str);
static void	  split_string(arg_t* args, char const* const str, size_t len);
static size_t get_next_word_index(char const* const str, size_t i);
static size_t get_word_end_index(char const* const str, size_t i);
static char*  sp_substr(char const* const src, size_t start, size_t len);

arg_t arg_split(char const* const src) {
	arg_t args;

	size_t len = init_args(&args, src);
	if (len == 0) {
		return (args);
	}

	if (init_argv(&args) == 0) {
		return (args);
	}

	split_string(&args, src, len);

	return (args);
}

static size_t init_args(arg_t* args, char const* const src) {
	args->argc = 0;
	args->argv = NULL;

	if (src == NULL) {
		return (0);
	}
	return (arg_count(args, src));
}

static size_t arg_count(arg_t* args, const char* const str) {
	size_t len = 0;
	bool   is_word = false;

	for (; str[len] != '\0'; ++len) {
		if (is_word == true && (str[len] == ' ' || str[len] == '\t')) {
			is_word = false;
		} else if (is_word == false && str[len] != ' ' && str[len] != '\t') {
			is_word = true;
			++args->argc;
		}
	}
	return (len);
}

static size_t init_argv(arg_t* args) {
	args->argv = kpmalloc(args->argc + 1);
	if (args->argv == NULL) {
		args->argc = 0;
	} else {
		args->argv[args->argc] = NULL;
	}
	return (args->argc);
}

static void split_string(arg_t* args, char const* const str, size_t len) {
	size_t start;
	size_t end = 0;

	for (size_t i = 0; end < len; ++i) {
		start = get_next_word_index(str, end);
		if (start >= len) {
			break;
		}
		end = get_word_end_index(str, start);
		args->argv[i] = sp_substr(str, start, end - start);
		if (args->argv[i] == NULL) {
			arg_split_clean(args);
			break;
		}
	}
}

static char* sp_substr(char const* const src, size_t start, size_t len) {
	char* dest = kpmalloc(len + 1);
	if (dest == NULL) {
		return (NULL);
	}
	for (size_t i = 0; i < len; ++i) {
		dest[i] = src[start + i];
	}
	dest[len] = '\0';
	return (dest);
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
	for (size_t i = 0; args->argv[i] != 0; ++i) {
		kpfree(args->argv[i]);
	}
	kpfree(args->argv);
	args->argc = 0;
	args->argv = NULL;
}
