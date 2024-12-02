#include "pic.h"
#include "kernel.h"

void PIC_remap(uint8_t pic1_offset, uint8_t pic2_offset) {
	outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND);
	outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND);
	outb(pic1_offset, PIC1_DATA);
	outb(pic2_offset, PIC2_DATA);
	outb(4, PIC1_DATA);
	outb(2, PIC2_DATA);
	outb(ICW4_8086, PIC1_DATA);
	outb(ICW4_8086, PIC2_DATA);

	outb(0, PIC1_DATA);
	outb(0, PIC2_DATA);
}

void IRQ_set_mask(uint8_t IRQline, bool masked) {
	uint16_t port;
	uint8_t	 bitmap;

	if (IRQline < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		IRQline -= 8;
	}
	if (masked == true) {
		bitmap = inb(port) | (1 << IRQline);
	} else {
		bitmap = inb(port) & ~(1 << IRQline);
	}
	outb(bitmap, port);
}

void init_PIC() {
	PIC_remap(0x20, 0x28);
	IRQ_set_mask(IRQ_TIMER, UNMASKED);
	IRQ_set_mask(IRQ_KEYBOARD, UNMASKED);
	// IRQ_unmask_all();
}

void IRQ_unmask_all() {
	outb(0, PIC1_DATA);
	outb(0, PIC2_DATA);
}
void IRQ_mask_all() {
	outb(0xFF, PIC1_DATA);
	outb(0xFF, PIC2_DATA);
}
