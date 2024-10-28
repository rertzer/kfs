#include "kernel.h"

static inline bool is_alpha_code(uint8_t scancode);
static void		   handle_ascii(keypress_t* current);
static bool		   get_upper(keypress_t* current);
static void		   set_ascii(keypress_t* current, bool upper);
static void		   handle_fn(keypress_t* current);

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
		term_left();
	}
}

void handle_right(keypress_t* keypress) {
	if (keypress->control == TRUE) {
		switch_next_term();
	} else {
		term_right();
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

static void handle_ascii(keypress_t* current) {
	bool upper = get_upper(current);
	set_ascii(current, upper);
}

static bool get_upper(keypress_t* current) {
	bool upper = FALSE;
	if (current->shift && !current->capslock) {
		upper = TRUE;
	} else if (current->capslock && !current->shift && is_alpha_code(current->scancode)) {
		upper = TRUE;
	}
	return (upper);
}

static inline bool is_alpha_code(uint8_t scancode) {
	bool is = FALSE;

	if ((scancode >= 0x10 && scancode <= 0x19) || (scancode >= 0x1F && scancode <= 0x27) ||
		(scancode >= 0x2C && scancode <= 0x33)) {
		is = TRUE;
	}
	return (is);
}

static void set_ascii(keypress_t* current, bool upper) {
	static unsigned char const codes[125][2] = {KEYCODES_TO_QWERTY};
	current->ascii = codes[current->keycode][upper];
}

void handle_control(keypress_t* current) {
	if (current->pressed) {
		current->control = TRUE;
	} else {
		current->control = FALSE;
	}
}

void handle_shift(keypress_t* current) {
	if (current->pressed) {
		current->shift = TRUE;
	} else {
		current->shift = FALSE;
	}
}

void handle_alt(keypress_t* current) {
	if (current->pressed) {
		current->alt = TRUE;
	} else {
		current->alt = FALSE;
	}
}

void handle_capslock(keypress_t* current) {
	if (current->pressed) {
		current->capslock = !current->capslock;
	}
}

void handle_numlock(keypress_t* current) {
	if (current->pressed) {
		current->numlock = !current->numlock;
	}
}

void handle_scrolllock(keypress_t* current) {
	if (current->pressed) {
		current->scrolllock = !current->scrolllock;
	}
}

static void handle_fn(keypress_t* current) {
	if (current->scancode < 0x45) {
		current->keycode = current->scancode - 0x3A;
	} else {
		current->keycode = current->scancode - 0x4C;
	}
}
