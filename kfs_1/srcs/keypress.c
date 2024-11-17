#include "kernel.h"

bool handle_keypress(keypress_t keypress) {
	bool getline = false;
	if (keypress.control == true) {
		handle_control_keypress(keypress);
	} else {
		getline = handle_default_keypress(keypress);
	}
	return (getline);
}

void handle_control_keypress(keypress_t keypress) {
	if (keypress.ascii == 0) {
		handle_control_keycode(keypress.keycode);
	} else {
		handle_control_ascii(keypress.ascii);
	}
}

void handle_control_keycode(keycode_t keycode) {
	switch (keycode) {
		case LEFT:
			switch_previous_term();
			break;
		case RIGHT:
			switch_next_term();
			break;
		default:
			break;
	}
}

void handle_control_ascii(uint8_t ascii) {
	switch (ascii) {
		case '[':
			term_front_color_next();
			break;
		case ']':
			term_back_color_next();
		default:
			break;
	}
}

bool handle_default_keypress(keypress_t keypress) {
	bool getline = false;
	if (keypress.ascii == 0) {
		handle_default_keycode(keypress.keycode);
	} else {
		getline = handle_default_ascii(keypress.ascii);
	}
	return (getline);
}

void handle_default_keycode(keycode_t keycode) {
	switch (keycode) {
		case HOME:
			term_home();
			break;
		case END:
			term_end();
			break;
		case LEFT:
			term_left();
			break;
		case RIGHT:
			term_right();
			break;
		case UP:
			term_up();
			break;
		case DOWN:
			term_down();
			break;
		case DELETE:
			term_delete();
			break;
		default:
			break;
	}
}

bool handle_default_ascii(uint8_t ascii) {
	bool getline = false;
	term_putchar(ascii);
	if (ascii == '\n') {
		getline = true;
	}
	return (getline);
}

void term_readline() {}
