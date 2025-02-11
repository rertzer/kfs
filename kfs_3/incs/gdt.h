#ifndef GDT_H
#define GDT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GDT_BUFFER 0xc0000800

typedef struct {
	uint32_t base;
	uint32_t limit;
	uint8_t	 access;
	uint8_t	 flags;
} gdt_descriptor_t;

typedef union gdt_entry_u {
	uint64_t desc;
	uint8_t	 bytes[8];
} gdt_entry_t;

void init_gdt();
void set_gdt(uint32_t limit, uint32_t base);
#endif
