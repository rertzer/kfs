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
