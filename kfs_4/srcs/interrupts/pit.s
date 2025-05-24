; Programmable Interval Time
; channel 0

section .bss
global pit_total_ms
pit_total_ms : resd 1
global timer_counter
timer_counter : resd 1



section .data
pit_reload_value : dw 1193 ; frequency 1000 Hz


section .text

global init_pit
init_pit:
	pusha
	pushf
	cli

	mov al, 00110100b ;channel 0, lobyte/hibyte, rategenerator
	out 0x43, al

	mov ax, [pit_reload_value]
	out 0x40, al
	mov al, ah
	out 0x40, al

	popf
	popa
	ret

global reset_timer_counter
reset_timer_counter:
	cli
	mov [timer_counter], dword 0
	sti
	ret

global get_timer_counter
get_timer_counter:
	cli
	mov eax, dword [timer_counter]
	sti
	ret

global get_pit_total_ms
get_pit_total_ms:
	cli
	mov eax, dword [timer_counter]
	sti
	ret
