bits 32
; ===================== constants =========================
; multiboot constants
MALIGN equ 1 << 0
MEMINFO equ 1 << 1
MBFLAGS equ MALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

; linker constants
extern LOW_KERNEL_START
extern LOW_KERNEL_END
extern HIGH_KERNEL_START
extern HIGH_KERNEL_END
extern HIGH_KERNEL_SIZE

; kernel constants
PAGE_PERM equ 0x00000003			;supervisor read/write present
PAGE_SIZE equ 4096
extern kernel_main


; =================== low kernel ==========================
; multiboot header that marks the program as a kernel.
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .low_kernel_bss nobits
alignb 4096
page_dir:
	resd 1024
low_kernel_page_table:
	resd 1024
high_kernel_page_table:
	resd 1024

section .low_kernel_data

section .low_kernel_text progbits
global _start
_start:
	; page directory table
	;low kernel entry
	global page_dir
	global low_kernel_page_table
	global high_kernel_page_table
	mov eax, low_kernel_page_table
	mov [page_dir], eax
	or dword [page_dir], PAGE_PERM
	;high kernel entry (3 Gb = 768 * 4 Mb)
	mov eax, high_kernel_page_table
	;mov [page_dir + 768 * 4], eax
	;self referencing dir table
	mov eax, page_dir
	;mov [page_dir + 1023 * 4], eax

	; low_kernel_page_table : identity mapping of the first 4 Mb
	; high kernel_page_table: first 4 Mb mapped to the high part (3 Gb)
	mov eax, 0x0		;table index
	mov ebx, 0x0		;physical address start 
	or ebx, PAGE_PERM	; add permissions
	page_table_init_loop:
	mov [low_kernel_page_table + eax * 4], ebx
	mov [high_kernel_page_table + eax * 4], ebx
	add ebx, PAGE_SIZE
	inc eax
	cmp eax, 1024
	jne page_table_init_loop

	; enable paging
	mov eax, page_dir
	mov cr3, eax			;load the page dir register 
	mov eax, 0x80000000
	mov cr0, eax			;set PG field

	; call the high kernel
	mov esp, stack_bottom	; load the stack pointer
	call kernel_main		; call the main kernel
	cli
.hang: hlt					; enter in an infinite loop
	jmp .hang

; ======================= high kernel sections ==================
section .data
section .bss
	align 16
	global stack_bottom
	global stack_top

	stack_top:
		resb 16384
	stack_bottom:

section .text

; ====================== dirty reboot ===============================	
global freboot
freboot:
	JMP 0xFFFF:0
