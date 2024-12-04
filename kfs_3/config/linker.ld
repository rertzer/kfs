ENTRY(_start)

SECTIONS
	{
	/* low kernel start address */
	. = 0x00100000; 
	LOW_KERNEL_START = .;

	/* low kernel section */ 
	lk_multiboot  : {*(.multiboot)}
	lk_text ALIGN (0x1000) : {*(.low_kernel_text)}
	lk_data ALIGN (0x1000) : {*(.low_kernel_data)}
	lk_bss ALIGN (0x1000) : {*(.low_kernel_bss)}
	LOW_KERNEL_END = .;

	/* high kernel section (3 Gb)*/
	. += 0xC0000000;
	HIGH_KERNEL_START = .;

	.text ALIGN(4K): /* AT(ADDR(.text) - 0xC0000000) */
		{
		*(.text)
	}
	.rodata  ALIGN(4K):  /*AT(ADDR(.rodata) - 0xC0000000)*/
	{
		*(.rodata)
	}
	.data ALIGN(4K):  /*AT(ADDR(.data) - 0xC0000000)*/
	{
		*(.data)
	}
	.bss ALIGN(4K):  /*AT(ADDR(.bss) - 0xC0000000)*/
	{
		*(COMMON)
		*(.bss)
	}

	HIGH_KERNEL_END = .;
	HIGH_HERNEL_SIZE = HIGH_KERNEL_END - HIGH_KERNEL_START;
}
