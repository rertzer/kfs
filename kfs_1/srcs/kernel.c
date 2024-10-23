#include "kernel.h"

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_idt();
	term_putstr("42\n");

	while (true) {
		uint8_t	   scancode = get_keyboard_input();
		keypress_t keypress = handle_scancode(scancode);
		if (keypress.pressed == TRUE) {
			handle_keypress(&keypress);
			// if (keypress.ascii != 0) {
			// 	int a = 42 / (keypress.ascii - '0');
			// 	term_putchar(a);
			// }
		}
	}
}
