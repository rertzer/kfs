section .text

extern stack_bottom
global copy_registers_to_tss
global init_tss_registers
global load_task_register
global store_task_register
global task_switch

init_tss_registers:
	push ebp
	mov ebp, esp


	mov ecx, [ebp+8]				;retrieve first argument (tss address)
	mov dword [ecx + 4], stack_bottom		; esp0
	mov [ecx + 8], ss						;ss0 
	mov dword [ecx + 12], stack_bottom 		; esp1 
	mov [ecx + 16], ss						; ss1 
	mov dword [ecx + 20], stack_bottom		; esp2 
	mov [ecx + 24], ss						; ss2 
	mov eax, cr3
	mov [ecx + 28], eax						; cr3
	mov  dword [ecx + 32], 0				; eip
	pushf
	pop eax
	mov [ecx + 36], eax	; eflags
	mov dword [ecx + 40], 0		; eax
	mov dword [ecx + 44], 0		; ecx
	mov dword [ecx + 48], 0		; edx
	mov dword [ecx + 52], 0		; ebx
	mov dword [ecx + 56], stack_bottom		; esp
	mov dword [ecx + 60], stack_bottom		;ebp 
	mov dword [ecx + 64], 0 	; esi
	mov dword [ecx + 68], 0		; edi
	mov [ecx + 72], es		; es 
	mov [ecx + 76], cs		; cs 
	mov [ecx + 80], ss		; ss 
	mov [ecx + 84], ds		; ds 
	mov [ecx + 88], fs		; fs 
	mov [ecx + 92], gs		; gs 
	mov dword [ecx + 96], 0		; ldt + I/O map base

	mov esp, ebp
	pop ebp
	ret

copy_registers_to_tss:
	push ebp
	mov ebp, esp

	mov ecx, [ebp+8]			;retrieve first argument (tss address)

	mov [ecx + 4], esp		; esp0
	mov [ecx + 8], ss		;ss0 
	mov [ecx + 12], esp 	; esp1 
	mov [ecx + 16], ss		; ss1 
	mov [ecx + 20], esp		; esp2 
	mov [ecx + 24], ss		; ss2 
	mov eax, cr3
	mov [ecx + 28], eax		; cr3
	;mov [ecx + 32], eip		; eip
	pushf
	pop eax
	mov [ecx + 36], eax	; eflags
	mov [ecx + 40], eax		; eax
	mov [ecx + 44], ecx		; ecx
	mov [ecx + 48], edx		; edx
	mov [ecx + 52], ebx		; ebx
	mov [ecx + 56], esp		; esp
	mov [ecx + 60], ebp		;ebp 
	mov [ecx + 64], esi 	; esi
	mov [ecx + 68], edi		; edi
	mov [ecx + 72], es		; es 
	mov [ecx + 76], cs		; cs 
	mov [ecx + 80], ss		; ss 
	mov [ecx + 84], ds		; ds 
	mov [ecx + 88], fs		; fs 
	mov [ecx + 92], gs		; gs 
	
	mov esp, ebp
	pop ebp
	ret

load_task_register:
	push ebp
	mov ebp, esp

	mov word ax, [ebp+8]
	ltr ax 

	mov esp, ebp
	pop ebp
	ret

store_task_register:
	push ebp
	mov ebp, esp

	xor eax, eax
	str ax

	mov esp, ebp
	pop ebp
	ret


task_switch:
	push ebp
	mov ebp, esp
	
	xor eax, eax
	mov word ax, [ebp+8] ; retrieve gdt offset

	push word ax 
	push dword 0x00
	jmp far [esp]

	mov esp, ebp
	pop ebp
	ret
