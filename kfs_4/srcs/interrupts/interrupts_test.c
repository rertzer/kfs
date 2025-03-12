#include "interrupts.h"
#include "builtin.h"
#include "kernel.h"
#include "keycode.h"

void interrupts_test() {
	// zero_divide_test();
	// debug_exception_test();
	// nmi_test();
	// breakpoint_test();
	// overflow_test();
	// bound_test();
	// invalid_opcode_test();
	// coprocessor_not_available_test();
	double_fault_test();
}

void zero_divide_test() {
	volatile int zero = 42;
	press_any();
	printf("divide 42 / %d = %d\n", zero, 42 / zero);
	zero = 0;
	press_any();
	printf("divide 42 / %d = %d\n", zero, 42 / zero);
}

void debug_exception_test() {
	asm volatile("int $0x1");
}

void nmi_test() {
	printf("non maskable interrupt test\n");
	press_any();
	asm volatile("int $0x2");
}

void breakpoint_test() {
	printf("breakpoint test\n");
	press_any();
	asm volatile("int3");
}

void overflow_test() {
	printf("overflow test INTO without overflow\n");
	press_any();
	volatile int32_t a = 0x7FFFFFFE;
	a += 1;
	asm volatile("into");
	printf("0x7FFFFFFE + 1 = %d\n", a);
	printf("overflow test INTO with overflow\n");
	press_any();
	a += 1;
	asm volatile("into");
	printf("0x7FFFFFFF + 1 = %d\n", a);
	asm volatile("into");
}

void bound_test() {
	printf("test bound instruction\n");
	press_any();
	asm_bound_test();
	printf("inside bound\n");
}

void invalid_opcode_test() {
	printf("test invalid opcode\n");
	press_any();
	asm volatile(".byte 0x0F, 0xFF");  // Invalid opcode
}

// The processor executed a WAIT/FWAIT instruction while the MP and TS flags of register CR0 were set
void coprocessor_not_available_test() {
	printf("coprocessor_not_available_test\n");
	press_any();
	asm_coprocessor_not_available_test();
}

void double_fault_test() {
	printf("double fault test\n");
	printf("please uncommment lines 21 and 22 in interrupt.c\n");
	press_any();
	// asm volatile("int $0x15");	// error 11
	asm volatile("int $0xd");
}
