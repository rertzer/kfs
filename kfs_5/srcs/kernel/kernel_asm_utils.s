bits 32
; keyboard I/O
KBRD_INTRFC equ 0x64
KBRD_RESET equ 0xFE
KBRD_UDATA equ 0x02

section .text
%macro registers_reset 0
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor esi, esi
	xor edi, edi
	xor ebp, ebp
	xor esp, esp
%endmacro	


global kbrd_asm_reboot
kbrd_asm_reboot:
	registers_reset
	mov eax, KBRD_RESET
	out KBRD_INTRFC, eax 
	hlt
	ret
	

global qemu_asm_shutdown
qemu_asm_shutdown:
	registers_reset
	mov eax, 0x2000
	mov edx, 0x604
	out  dx, ax 
	hlt
	ret
