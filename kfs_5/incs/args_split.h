#ifndef ARGS_SPLIT_H
#define ARGS_SPLIT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t argc;
	char** argv;
} arg_t;

arg_t arg_split(char const* const str);
void  arg_split_clean(arg_t* args);
bool  arg_split_test();
char* jr_substr(char const* const src, size_t start, size_t len);
char* term_substr(uint16_t const* const src, size_t start, size_t len);

#endif	// ARGS_SPLIT_H
