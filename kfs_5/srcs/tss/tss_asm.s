section .text

global copy_registers_to_tss
copy_registers_to_tss:
	mov ecx, [esp+4]			;retrieve first argument (tss address)
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
	; mov [ecx + 96],		; ldt 
	; mov [ecx + 100],		; 
	; mov [ecx + 104],		; 
	; mov [ecx + 108],		; 
	 


