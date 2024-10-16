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
	stack_bottom:
		resb 16384
	stack_top:

; start function: load the stack pointer, call the main kernel
; and enter in an infinite loop
section .text
	global _start:function (_start.end - _start)
_start:
	mov esp, stack_top
	extern kernel_main
	call kernel_main
	cli
.hang: hlt
	jmp .hang
.end:

global gdt_flush
gdt_flush:
	mov eax, [esp + 4]
	lgdt [eax]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax
	jmp 0x08:.flush

.flush:
	ret
