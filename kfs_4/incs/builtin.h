#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>
#include "boot_infos.h"
#include "kernel.h"

#define REGS_NB 18

#define BUILTINS                                                                                           \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [REBOOT] = "reboot", [HALT] = "halt",       \
	[REGISTERS] = "registers", [HEXDUMP] = "hexdump", [READDUMP] = "readdump", [BOOT_INFOS] = "bootinfos", \
	[MEMORY_INFOS] = "memoryinfos", [INTERRUPT] = "interrupt",

#define BUILTIN_FUNCTIONS                                                                                           \
	[ECHO] = echo, [QWERTY] = qwerty, [AZERTY] = azerty, [REBOOT] = reboot, [HALT] = halt, [REGISTERS] = registers, \
	[HEXDUMP] = hexdump, [READDUMP] = readdump, [BOOT_INFOS] = boot_infos, [MEMORY_INFOS] = memory_infos,           \
	[INTERRUPT] = interrupt

typedef enum {
	ECHO,
	QWERTY,
	AZERTY,
	REBOOT,
	HALT,
	REGISTERS,
	HEXDUMP,
	READDUMP,
	BOOT_INFOS,
	MEMORY_INFOS,
	INTERRUPT,
	BUILTINS_NB
} builtin_t;

typedef uint8_t (*builtin_fun_t)(size_t argc, char** argv);

bool  process_line();
char* readline();

uint8_t echo(size_t argc, char** argv);
uint8_t qwerty(size_t argc, char** argv);
uint8_t azerty(size_t argc, char** argv);
uint8_t reboot(size_t argc, char** argv);
uint8_t halt(size_t argc, char** argv);
uint8_t registers(size_t argc, char** argv);
uint8_t boot_infos(size_t argc, char** argv);
uint8_t hexdump(size_t argc, char** argv);
uint8_t readdump(size_t argc, char** argv);
uint8_t memory_infos(size_t argc, char** argv);
uint8_t interrupt(size_t argc, char** argv);

void	 freboot();
uint32_t boom();
uint32_t get_retaddr();
void	 tabledump();
void	 print_mem_infos(mem_info_t* mem_infos);
#endif
