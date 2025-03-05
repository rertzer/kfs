#include "interrupts.h"
#include "builtin.h"
#include "kernel.h"
#include "keycode.h"

void interrupts_test() {
	// zero_divide_test();
	// debug_exception_test();
	// nmi_test();
	// breakpoint_test();
	overflow_test(0xFFFFFFFF);
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

void overflow_test(uint32_t over) {
	printf("overflow test INTO without overflow\n");
	press_any();
	volatile uint32_t a = 0xFFFFFFF0;
	a += over;
	asm volatile("into");
	printf("overflow test INTO with overflow\n");
	press_any();
	a -= 2;
	uint32_t		  flags = 0;
	volatile uint32_t b = 0;
	asm volatile(
		" \
	mov $0xFFFFFFFF, %%eax; \
	add %2, %%eax;\
	into; \
	mov %%eax, %0; \
	pushf;\
	pop %%ebx;\
	mov %%ebx, %1;"
		: "=a"(b), "=b"(flags)
		: "r"(over)
		: "memory");
	printf("a= %u b = %u flags: %x\n", a, b, flags);
}
