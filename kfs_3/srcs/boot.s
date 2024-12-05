bits 32
; multiboot constant header

MALIGN equ 1 << 0
MEMINFO equ 1 << 1
MBFLAGS equ MALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

; multiboot header that marks the program as a kernel.
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

; creating a stack

section .bss
	align 16
	global stack_bottom
	global stack_top

	stack_top:
		resb 16384
	stack_bottom:

section .text
; start function: load the stack pointer, call the main kernel
; and enter in an infinite loop
global _start:function (_start.end - _start)
_start:
	mov esp, stack_bottom
	extern kernel_main
	call kernel_main
	cli
.hang: hlt
	jmp .hang
.end:

global freboot
freboot:
	JMP 0xFFFF:0

global clean_registers
clean_registers:
	xor esp, esp
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	xor ebp, ebp
	ret