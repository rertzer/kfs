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
PAGING_ENABLED equ 0x80000000
	WRITE_PROTECTED equ 1 << 16
PAGE_PERM equ 0x00000003			;supervisor read/write present
PAGE_SIZE equ 4096
extern kernel_main
extern godot
extern get_registers
extern invalidate_low_kernel
extern flush_tlb
extern multiboot_magic
extern multiboot_tags


; =================== low kernel ==========================
; multiboot header that marks the program as a kernel.
section .multiboot.data
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .multiboot.bss nobits
; paging
alignb 4096
page_dir:
	resd 1024
low_kernel_page_table:
	resd 1024
; multiboot infos
multiboot_magic:
	resd 1
multiboot_tags:
	resd 1


section .multiboot.text progbits
global _start
_start:
	; store multiboot infos
	mov [multiboot_magic], eax
	mov [multiboot_tags], ebx
	; page directory table
	;low kernel entry_
	global page_dir
	global low_kernel_page_table

	mov eax, low_kernel_page_table
	or dword eax, PAGE_PERM
	mov [page_dir], eax
	;high kernel entry (3 Gb = 768 * 4 Mb)
	mov [page_dir + 768 * 4], eax
	;self referencing dir table
	mov eax, page_dir
	or eax, PAGE_PERM
	mov [page_dir + 1023 * 4], eax

	; low_kernel_page_table : identity mapping of the first 4 Mb
	; high kernel_page_table: first 4 Mb mapped to the high part (3 Gb)
	mov eax, 0x0		;table index
	mov ebx, 0x0		;physical address start 
	or ebx, PAGE_PERM	; add permissions
	.page_table_init_loop:
	mov [low_kernel_page_table + eax * 4], ebx
	add ebx, PAGE_SIZE
	inc eax
	cmp eax, 1024
	jne .page_table_init_loop

	; enable paging and set write protection 
	mov eax, page_dir
	mov cr3, eax			;load the page dir register 
	mov eax, cr0
	or eax, PAGING_ENABLED 
	or eax, WRITE_PROTECTED
	mov cr0, eax			;set PG field

	; switch to high kernel addresses
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
	global p_mmap_start
	global p_mmap_end
	global user_v_mmap_start
	global user_v_mmap_end
	global kernel_v_mmap_start
	global kernel_v_mmap_end

	stack_top:
		resb 16384
	stack_bottom:

	p_mmap_start:
		resb 524288 
	p_mmap_end:
	user_v_mmap_start:
		resb 393216 
	user_v_mmap_end:
	kernel_v_mmap_start:
		resb 131072
	kernel_v_mmap_end:
	


section .text

; ====================== kernel utils ==============================
global freboot
freboot:
	JMP 0xFFFF:0


global get_retaddr
get_retaddr:
    mov  eax, [esp]
    ret 

godot:
	.waiting_for_godot:
	hlt
	jmp .waiting_for_godot
	ret
	
invalidate_low_kernel:
	mov dword [page_dir], 0x0
    invlpg [0]
	ret

flush_tlb:
	mov eax, page_dir
	mov cr3, eax			;load the page dir register 
	mov eax, cr0
	ret

get_registers:
	push ebp
	mov ebp, esp

	mov ecx, [ebp+8]
	mov [ecx], eax
	;add ecx, 4
	mov [ecx + 4], ebx
	mov [ecx + 8], ecx
	mov [ecx + 12], edx
	mov [ecx + 16], ebp
	mov [ecx + 20], esp
	mov [ecx + 24], esi
	mov [ecx + 28], edi
	mov [ecx + 32], ss
	mov [ecx + 36], ds 
	mov [ecx + 40], es
	mov [ecx + 44], fs 
	mov [ecx + 48], gs 
	mov [ecx + 52], cs 
	mov eax, cr0
	mov [ecx + 56], eax
	mov eax, cr3
	mov [ecx + 60], eax
	pushf
	pop eax
	mov [ecx + 64], eax 

	mov esp, ebp
	pop ebp
	ret


;section .multiboot.text
global boom
boom:
	push 0x08
	lea eax, [theend]
	push eax 
	retf	

theend:
	ret

