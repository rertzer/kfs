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
			handle_left(keypress);
			break;
		case RIGHT:
			handle_right(keypress);
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
		if (keypress->control == TRUE) {
			handle_control_ascii(keypress->ascii);
		} else {
			term_putchar(keypress->ascii);
		}
	}
}

void handle_left(keypress_t* keypress) {
	if (keypress->control == TRUE) {
		switch_previous_term();
	} else {
		term_previous();
	}
}

void handle_right(keypress_t* keypress) {
	if (keypress->control == TRUE) {
		switch_next_term();
	} else {
		term_next();
	}
}
void handle_home() {}

void handle_end() {}

void handle_delete() {
	term_putchar(' ');
}
void handle_control_ascii(uint8_t c) {
	switch (c) {
		case '[':
			term_front_color_next();
			break;
		case ']':
			term_back_color_next();
		default:
			break;
	}
}
