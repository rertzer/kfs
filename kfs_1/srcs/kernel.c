#include "kernel.h"

extern volatile uint8_t current_scancode;

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_idt();
	term_putstr("42\n");

	while (true) {
		printk("looping\n");
		sleep();
		printk("scancode %d\n", current_scancode);
		//	keypress_t keypress = handle_scancode(scancode);
		// if (keypress.pressed == TRUE) {
		// 	handle_keypress(&keypress);
		//
		// }
	}
}
