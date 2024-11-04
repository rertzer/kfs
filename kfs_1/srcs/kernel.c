#include "kernel.h"

extern volatile uint8_t current_code;

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_idt();
	term_putstr("42\n");

	keypress_t keypress = init_keypress();

	while (true) {
		// printk("looping\n");
		sleep();
		keypress.scancode = current_code;
		if (keypress.scancode != 0) {
			keypress = update_keypress(keypress);
			if (keypress.pressed == PRESSED) {
				handle_keypress(keypress);
			}
		}
	}
}