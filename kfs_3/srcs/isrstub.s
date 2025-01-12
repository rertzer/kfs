; ===================== DATA ===============================
extern page_fault_handler
extern general_protection_handler
extern exception_handler
extern keyboard_handler
extern pit_total_ms
extern timer_counter


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

; ==================== TEXT ===============================
section .text

%macro isr_err_stub 1
isr_stub_%+%1:
	cli
	pusha
	mov eax, %+%1
	mov [hereafter], eax ;%+%1
    call exception_handler
	mov	al, 0x20	; set bit 4 of OCW 2
	out	0x20, al	; write to primary PIC command register
	popa
	sti
    iret 
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
	cli
	pusha
	mov	al, 0x20	; set bit 4 of OCW 2
	out	0x20, al	; write to primary PIC command register
	mov eax, %+%1
	mov [hereafter], eax;%+%1
    call exception_handler
	; send EOI to primary PIC
	popa
	sti
    iret
%endmacro


; general protection fault
isr_stub_13:
	cli
	pusha
	call general_protection_handler
	hlt
	popa
	sti
	iret

; page fault exception 
isr_stub_14:
	cli
	mov eax, cr2
	push eax
	call page_fault_handler
	pop eax
	sti
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
	
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
;isr_err_stub    13
; isr_err_stub    14
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
;isr_no_err_stub 32 ;timer
;isr_no_err_stub 33 ;keyboard
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47
