STACK_BASE_MASK equ 0x3FFF
section .text

global exec_asm 

exec_asm:
	mov eax, [esp+4]  ; first arg, fun addr
	mov ebx, [esp+8]  ; second arg, argc
	mov ecx, [esp+12] ; third arg, argv
	or ebp, STACK_BASE_MASK
	or esp, STACK_BASE_MASK
	push ecx
	push ebx
	call eax	
	ret ; should not return unless an error occured
