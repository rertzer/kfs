#ifndef EXEC_H
#define EXEC_H
#include <stddef.h>
#include <stdint.h>

typedef uint8_t (*exec_fun_t)(size_t argc, char** argv);

void exec_fn(exec_fun_t fun, size_t argc, char** argv);
int	 exec(exec_fun_t fun, size_t argc, char** argv);
int	 exec_asm(exec_fun_t fun, size_t argc, char** argv);

uint8_t test_exec(size_t argc, char** argv);
uint8_t test_exec_2(size_t argc, char** argv);

#endif
