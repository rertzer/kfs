gdtr DW 0 ; For limit storage
     DD 0 ; For base storage

section .text
global set_gdt
set_gdt:
	CLI
	MOV		AX, [esp + 4]
	MOV		[gdtr], AX
	MOV		EAX, [ESP + 8]
	MOV		[gdtr + 2], EAX
	LGDT	[gdtr]

	; Reload CS register containing code selector:
   JMP   0x08:.reload_CS ; kernel code segment
.reload_CS:
   ; Reload data and stack segment registers:
	MOV		AX, 0x10 ; kernel data segment
	MOV		DS, AX
	MOV		ES, AX
	MOV		FS, AX
	MOV		GS, AX
	MOV		AX, 0x18 ; kernel stack segment 
	MOV		SS, AX
	STI
	RET

