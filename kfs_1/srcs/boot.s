bits 32
; multiboot constant header
MALIGN equ 1 << 0
MEMINFO equ 1 << 1
MBFLAGS equ MALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)


extern exception_handler

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


section .data
align 32
global hereafter
hereafter:
dd 999 

global isr_stub_table
isr_stub_table:
dd isr_stub_0
dd isr_stub_1
dd isr_stub_2
dd isr_stub_3
dd isr_stub_4
dd isr_stub_5
dd isr_stub_6
dd isr_stub_7
dd isr_stub_8
dd isr_stub_9
dd isr_stub_10
dd isr_stub_11
dd isr_stub_12
dd isr_stub_13
dd isr_stub_14
dd isr_stub_15
dd isr_stub_16
dd isr_stub_17
dd isr_stub_18
dd isr_stub_19
dd isr_stub_20
dd isr_stub_21
dd isr_stub_22
dd isr_stub_23
dd isr_stub_24
dd isr_stub_25
dd isr_stub_26
dd isr_stub_27
dd isr_stub_28
dd isr_stub_29
dd isr_stub_30
dd isr_stub_31

;%assign i 0 
;%rep    32 
;    dd isr_stub_%+i
;%assign i i+1 
;%endrep

section .text
; start function: load the stack pointer, call the main kernel
; and enter in an infinite loop
global _start:function (_start.end - _start)
_start:
	mov esp, stack_top
	extern kernel_main
	call kernel_main
	cli
.hang: hlt
	jmp .hang
.end:



%macro isr_err_stub 1
isr_stub_%+%1:
	mov eax, %+%1
	mov [hereafter], eax ;%+%1
    call exception_handler
    iret 
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
	mov eax, %+%1
	mov [hereafter], eax;%+%1
    call exception_handler
    iret
%endmacro

isr_stub_8:
	iret
 
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
;isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31
