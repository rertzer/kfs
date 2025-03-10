#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>
#include "boot_infos.h"
#include "kernel.h"

#define REGS_NB 17

#define BUILTINS                                                                                     \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [REBOOT] = "reboot", [HALT] = "halt", \
	[REGISTERS] = "registers", [HEXDUMP] = "hexdump", [BOOT_INFOS] = "bootinfos", [MEMORY_INFOS] = "memoryinfos",

#define BUILTIN_FUNCTIONS                                                                                           \
	[ECHO] = echo, [QWERTY] = qwerty, [AZERTY] = azerty, [REBOOT] = reboot, [HALT] = halt, [REGISTERS] = registers, \
	[HEXDUMP] = hexdump, [BOOT_INFOS] = boot_infos, [MEMORY_INFOS] = memory_infos

typedef enum {
	ECHO,
	QWERTY,
	AZERTY,
	REBOOT,
	HALT,
	REGISTERS,
	HEXDUMP,
	BOOT_INFOS,
	MEMORY_INFOS,
	BUILTINS_NB
} builtin_t;

typedef struct {
	char   cmd[256];
	size_t cmd_len;
	char   arg[256];
	size_t arg_len;
} cmdline_t;

typedef uint8_t (*builtin_fun_t)(char* pointer, size_t len);

bool	 readline();
uint8_t	 echo(char* word, size_t word_len);
uint8_t	 qwerty(char* pointer, size_t len);
uint8_t	 azerty(char* pointer, size_t len);
uint8_t	 reboot(char* pointer, size_t len);
uint8_t	 halt(char* pointer, size_t len);
uint8_t	 registers(char* pointer, size_t len);
uint8_t	 boot_infos(char* pointer, size_t len);

void	 print_mem_infos(mem_info_t* mem_infos);
uint8_t	 memory_infos(char* pointer, size_t len);

void	 freboot();
uint32_t boom();
uint32_t get_retaddr();
uint8_t	 hexdump(char* pointer, size_t len);
void	 tabledump();

#endif
