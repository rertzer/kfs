#include "kernel.h"

void handle_keypress(keypress_t* keypress) {
	switch (keypress->keycode) {
		case R_CONTROL:
			handle_control(keypress);
			break;
		case R_ALT:
			handle_alt(keypress);
			break;
		case HOME:
			handle_home();
			break;
		case UP:
			term_previous_row();
			break;
		case PAGE_UP:
			break;
		case LEFT:
			term_previous();
			break;
		case RIGHT:
			term_next();
			break;
		case END:
			handle_end();
			break;
		case DOWN:
			term_next_row();
			break;
		case PAGE_DOWN:
			break;
		case DELETE:
			handle_delete();
			break;
		default:
			break;
	}
	if (keypress->ascii != 0) {
		term_putchar(keypress->ascii);
	}
}

void handle_home() {}

void handle_end() {}

void handle_delete() {
	term_putchar(' ');
}
