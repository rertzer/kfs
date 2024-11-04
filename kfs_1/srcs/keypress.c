#include "kernel.h"

void handle_keypress(keypress_t keypress) {
	if (keypress.control == true) {
		handle_control_keypress(keypress);
	} else {
		handle_default_keypress(keypress);
	}
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

void handle_default_keypress(keypress_t keypress) {
	if (keypress.ascii == 0) {
		handle_default_keycode(keypress.keycode);
	} else {
		handle_default_ascii(keypress.ascii);
	}
}

void handle_default_keycode(keycode_t keycode) {
	switch (keycode) {
		case HOME:
			handle_home();
			break;
		case END:
			handle_end();
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
			term_putchar(' ');
			break;
		case PAGE_UP:
			printk("%d\n", printk("%#010x\n", 42));
			printk("%d\n", printk("%#10x\n", 42));
			break;
		default:
			break;
	}
}

void handle_default_ascii(uint8_t ascii) {
	term_putchar(ascii);
}

void handle_home() {}

void handle_end() {}
