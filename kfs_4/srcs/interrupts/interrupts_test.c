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
	// double_fault_test();
	// copro_segment_test();
	// invalid_tss_test();
	// segment_not_present_test();
	// stack_exception_test();
	// general_protection_test();
	//	coprocessor_error_test();
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
	printf("coprocessor not available test\n");
	press_any();
	asm_coprocessor_not_available_test();
}

void double_fault_test() {
	printf("double fault test\n");
	printf("please uncommment lines 21 and 22 in interrupt.c\n");
	press_any();
	asm volatile("int $0xd");
}
void copro_segment_test() {
	printf("coprocessor segement overrun test\n");
	press_any();
	asm volatile("int $0x09");
}

void invalid_tss_test() {
	printf("invalid tss test\n");
	press_any();
	asm volatile("int $0xa");
}
/* interrupt 0x15 has been marked as NOT PRESENT; calling it triggers an
 * segment not present exception. See interrupt.c line 20
 * */
void segment_not_present_test() {
	printf("segment not present test\n");
	press_any();
	asm volatile("int $0x15");
}

void general_protection_test() {
	printf("general protection test\n");
	press_any();
	boom();
}

void stack_exception_test() {
	printf("stack exception test\n");
	press_any();
	asm_stack_exception_test();
	// asm volatile(
	/* 	"\
		mov $0x18, AL;\
		;mov SS, AL");
	 asm volatile("int $0xc"); */
}

void coprocessor_error_test() {
	printf("coprocessor error test\n");
	press_any();
	asm volatile("int $0x10");
}
