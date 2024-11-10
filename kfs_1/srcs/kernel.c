#include "kernel.h"

extern volatile uint8_t current_code;

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_idt();

	keypress_t keypress = init_keypress();

	term_putstr("42\n");
	term_prompt();
	while (true) {
		// printk("looping\n");
		sleep();
		bool getline = false;
		keypress.keycode = current_code;
		if (keypress.keycode != 0) {
			keypress = update_keypress(keypress);
			if (keypress.pressed == PRESSED) {
				getline = handle_keypress(keypress);
			}
		}
		if (getline == true) {
			readline();
			term_prompt();
		}
	}
}
