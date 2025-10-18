#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdint.h>
#include "boot_infos.h"
#include "kernel.h"

#define REGS_NB 18

#define BUILTINS                                                                                                 \
	[ECHO] = "echo", [QWERTY] = "qwerty", [AZERTY] = "azerty", [REBOOT] = "reboot", [HALT] = "halt",             \
	[REGISTERS] = "registers", [HEXDUMP] = "hexdump", [READDUMP] = "readdump", [BOOT_INFOS] = "bootinfos",       \
	[MEMORY_INFOS] = "memoryinfos", [INTERRUPT] = "interrupt", [PS] = "ps", [TEST_STACK_DUMP] = "testStackDump", \
	[UP_TIME] = "uptime",

#define BUILTIN_FUNCTIONS                                                                                           \
	[ECHO] = echo, [QWERTY] = qwerty, [AZERTY] = azerty, [REBOOT] = reboot, [HALT] = halt, [REGISTERS] = registers, \
	[HEXDUMP] = hexdump, [READDUMP] = readdump, [BOOT_INFOS] = boot_infos, [MEMORY_INFOS] = memory_infos,           \
	[INTERRUPT] = interrupt, [PS] = ps, [TEST_STACK_DUMP] = test_stack_dump, [UP_TIME] = uptime,

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
	PS,
	TEST_STACK_DUMP,
	UP_TIME,
	BUILTINS_NB
} builtin_t;

typedef enum {
	REG_EAX,
	REG_EBX,
	REG_ECX,
	REG_EDX,
	REG_EBP,
	REG_ESP,
	REG_ESI,
	REG_EDI,
	REG_SS,
	REG_DS,
	REG_ES,
	REG_FS,
	REG_GS,
	REG_CS,
	REG_CR0,
	REG_CR2,
	REG_CR3,
	REG_EFLAGS,
} register_e;

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
uint8_t ps(size_t argc, char** argv);
uint8_t test_stack_dump(size_t argc, char** argv);
uint8_t uptime(size_t argc, char** argv);

void	 freboot();
uint32_t get_retaddr();
void	 tabledump();
void	 print_mem_infos(mem_info_t* mem_infos);
#endif
