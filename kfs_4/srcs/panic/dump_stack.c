#include "panic.h"
#include "stdio.h"

volatile char* dump_addr = (char*)0xC0110000;

void dump_stack() {
	*dump_addr = 'B';
}

void read_dump() {
	printf("recovered value: %c\n", *dump_addr);
}
