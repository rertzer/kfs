; ===================== DATA ===============================
extern page_fault_handler
extern general_protection_handler
extern exception_handler
extern interrupt_handler
extern default_exception_handler
extern error_exception_handler
extern keyboard_handler
extern pit_total_ms
extern timer_counter
extern hello
extern flush_tlb


section .data
align 32
global hereafter
hereafter:
	dd 999 

global isr_stub_table
isr_stub_table:
	%assign i 0 
	%rep    48 
	dd isr_stub_%+i
	%assign i i+1 
	%endrep

global int_builtin_table
int_builtin_table:
	%assign i 0
	%rep 48
	dd int_builtin_%+i
	%assign i i+1
	%endrep
; ==================== TEXT ===============================
section .text

%macro isr_default_stub 1
isr_stub_%+%1:
	cli
	pusha
	mov eax, %+%1
	push eax
    call default_exception_handler
	add esp, 4	
	popa
	sti
    iret 
%endmacro

%macro isr_error_stub 1
isr_stub_%+%1:
	cli
	pushad
	push dword [esp+32]
	mov eax, %+%1
	push eax
    call error_exception_handler
	add esp, 8	
	popad
	sti
	add esp, 4
    iret 
%endmacro

%macro isr_interrupt_stub 1
isr_stub_%+%1:
	cli
	pusha
	mov eax, %+%1
	push eax
    call interrupt_handler
	add esp, 4	
	popa
	mov	al, 0x20	; set bit 4 of OCW 2
	out	0x20, al	; write to primary PIC command register
	sti
    iret
%endmacro


; page fault exception 
isr_stub_14:
	;cli
	pushad
	push dword [esp+32]
	mov eax, cr2
	push eax
	call page_fault_handler
	add esp, 8
	popad
	;sti
	add esp, 4	
	iret

; timer interrupt
isr_stub_32:
	cli
	pusha

	inc dword [pit_total_ms]
	inc dword [timer_counter]
	

	mov	al, 0x20	; set bit 4 of OCW 2
	out	0x20, al	; write to primary PIC command register 
	popa
	sti

	iret

; keyboard interrupt
isr_stub_33:
	cli
	pusha
	xor eax, eax
	in al, 0x64		; read keyboard status
	and al, 0x01   ; if bit 1 is set there is an input to read
	cmp al, 0x01
	jnz key_int_end
	in al, 0x60		; read keyboard value
	push eax	
	call keyboard_handler	
	key_int_end:
	pop eax
	mov	al, 0x20	; set bit 4 of OCW 2
	out	0x20, al	; write to primary PIC command register 
	popa
	sti
	iret

%assign i 0
%rep 10 
isr_default_stub i
%assign i i+1
%endrep

%assign i 10
%rep 4 
isr_error_stub i
%assign i i+1
%endrep

%assign i 15
%rep 17 
isr_default_stub i
%assign i i+1
%endrep

%assign i 34 
%rep 14 
isr_interrupt_stub i
%assign i i+1
%endrep

;isr_default_stub 0
;isr_default_stub 1
;isr_default_stub 2
;isr_default_stub 3
;isr_default_stub 4
;isr_default_stub 5
;isr_default_stub 6
;isr_default_stub 7
;isr_default_stub 8
;isr_default_stub 9
;isr_error_stub  10
;isr_error_stub  11
;isr_error_stub  12
;isr_error_stub  13
;isr_default_stub 15
;isr_default_stub 16
;isr_default_stub 17
;isr_default_stub 18
;isr_default_stub 19
;isr_default_stub 20
;isr_default_stub 21
;isr_default_stub 22
;isr_default_stub 23
;isr_default_stub 24
;isr_default_stub 25
;isr_default_stub 26
;isr_default_stub 27
;isr_default_stub 28
;isr_default_stub 29
;isr_default_stub 30
;isr_default_stub 31
;isr_interrupt_stub 34
;isr_interrupt_stub 35
;isr_interrupt_stub 36
;isr_interrupt_stub 37
;isr_interrupt_stub 38
;isr_interrupt_stub 39
;isr_interrupt_stub 40
;isr_interrupt_stub 41
;isr_interrupt_stub 42
;isr_interrupt_stub 43
;isr_interrupt_stub 44
;isr_interrupt_stub 45
;isr_interrupt_stub 46
;isr_interrupt_stub 47

%macro int_builtin_stub 1
int_builtin_%+%1:
	int %1
    ret
%endmacro

%assign i 0
%rep 48
int_builtin_stub i
%assign i i+1
%endrep


; ------------------------------------------ test -----------------------------------------------
MATH_PRESENT equ 1 << 1
TASK_SWITCHTED equ 1 << 3 

section .data
align 32
bound_test_limits:
	dd 1
	dd 9

section .text
global asm_bound_test
asm_bound_test:
	pusha
	mov eax, 55
	lea esi, [bound_test_limits]
	mov ebx, esi
	bound eax, [esi] 
	popa
	ret

global asm_coprocessor_not_available_test
asm_coprocessor_not_available_test:
	pusha
	; set control register such that a WAIT will trigger an exception
	mov eax, cr0
	or eax, MATH_PRESENT 
	or eax,	TASK_SWITCHTED 
	mov cr0, eax	
	; trigger exception 7
	wait
	popa
	ret
