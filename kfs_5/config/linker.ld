ENTRY(_start)


HIGH_SHIFT = 0xC0000000;

SECTIONS
	{
	/* low kernel start address */
	. = 0x00110000; 
	LOW_KERNEL_START = .;

	/* low kernel section */ 
	.multiboot.data  ALIGN (0x1000) : {*(.multiboot.data)}
	.multiboot.text ALIGN (0x1000) : {*(.multiboot.text)}
	.multiboot.bss ALIGN (0x1000) : {*(.multiboot.bss)}
	LOW_KERNEL_END = .;

	/* high kernel section (3 Gb)*/
	. += HIGH_SHIFT;
	HIGH_KERNEL_START = .;

	.text ALIGN(4K):  AT(ADDR(.text) - HIGH_SHIFT)
	{
		*(.text)
	}
	.rodata  ALIGN(4K):  AT(ADDR(.rodata) - HIGH_SHIFT)
	{
		*(.rodata)
	}
	.data ALIGN(4K):  AT(ADDR(.data) - HIGH_SHIFT)
	{
		*(.data)
	}
	.bss ALIGN(4K):  AT(ADDR(.bss) - HIGH_SHIFT)
	{
		*(COMMON)
		*(.bss)
	}

	HIGH_KERNEL_END = .;
	HIGH_KERNEL_SIZE = HIGH_KERNEL_END - HIGH_KERNEL_START;
}
