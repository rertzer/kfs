#include "kernel.h"

extern volatile uint8_t current_code;

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_pit();
	init_idt();

	keypress_t keypress = init_keypress();

	term_putstr("echo 42\n");
	readline();
	term_prompt();
	while (true) {
		sleep();
		// uint32_t time = get_timer_counter();
		// if (time % 1000 == 0) {
		// 	printk("time: %d\n", time);
		// 	if (time % 10000 == 0) {
		// 		reset_timer_counter();
		// 	}
		// }
		bool getline = false;
		keypress.keycode = current_code;
		current_code = 0;
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