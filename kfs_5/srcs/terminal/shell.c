#include "builtin.h"
#include "keycode.h"
#include "terminal.h"

extern volatile uint8_t current_code;

static void process_keyboard(keypress_t* keypress);

uint8_t shell(size_t argc, char** argv) {
	printk("jrOS ready. Welcome to kernel zero. Enjoy!\n");
	keypress_t keypress = init_keypress();
	term_prompt();
	while (true) {
		halting();
		process_keyboard(&keypress);
	}
	return (0);
}

static void process_keyboard(keypress_t* keypress) {
	bool getline = false;
	keypress->keycode = current_code;
	current_code = 0;
	if (keypress->keycode != 0) {
		update_keypress(keypress);
		if (keypress->pressed == PRESSED) {
			getline = handle_keypress(*keypress);
		}
	}
	if (getline == true) {
		process_line();
		term_prompt();
	}
}
