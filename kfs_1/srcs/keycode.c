#include "kernel.h"

static inline bool is_alpha_code(uint8_t scancode);
static uint8_t	   handle_ascii(keypress_t current);
static bool		   get_upper(keypress_t current);
static uint8_t	   get_ascii(keypress_t current, bool upper);
static keypress_t  handle_switches(keypress_t keypress);
static keypress_t  handle_control(keypress_t keypress);
static keypress_t  handle_alt(keypress_t keypress);
static keypress_t  handle_shift(keypress_t keypress);
static keypress_t  handle_gui(keypress_t keypress);
static keypress_t  handle_capslock(keypress_t keypress);
static keypress_t  handle_numlock(keypress_t keypress);
static keypress_t  handle_scrolllock(keypress_t keypress);
static void		   handle_fn(keypress_t* current);

keypress_t init_keypress() {
	keypress_t keypress;
	keypress.pressed = false;
	keypress.control = false;
	keypress.alt = false;
	keypress.shift = false;
	keypress.gui = false;
	keypress.capslock = false;
	keypress.numlock = false;
	keypress.scrolllock = false;
	keypress.scancode = 0;
	keypress.ascii = 0;
	keypress.keycode = 0;

	return (keypress);
}

keypress_t update_keypress(keypress_t keypress) {
	keypress.pressed = keypress.scancode >> 7;
	keypress.keycode = keypress.scancode & 0x7F;
	keypress.ascii = handle_ascii(keypress);
	keypress = handle_switches(keypress);
}

static keypress_t handle_switches(keypress_t keypress) {
	switch (keypress.keycode) {
		case L_CONTROL:
		case R_CONTROL:
			keypress = handle_control(keypress);
			break;
		case L_ALT:
		case R_ALT:
			keypress = handle_alt(keypress);
			break;
		case L_SHIFT:
		case R_SHIFT:
			keypress = handle_shift(keypress);
			break;
		case L_GUI:
		case R_GUI:
			keypress = handle_gui(keypress);
			break;
		case CAPSLOCK:
			keypress = handle_capslock(keypress);
			break;
		case NUMLOCK:
			keypress = handle_numlock(keypress);
			break;
		case SCROLLLOCK:
			keypress = handle_scrolllock(keypress);
			break;

		default:
			break;
	}
	return (keypress);
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

static uint8_t handle_ascii(keypress_t current) {
	bool upper = get_upper(current);
	return (get_ascii(current, upper));
}

static bool get_upper(keypress_t current) {
	bool upper = FALSE;
	if (current.shift && !current.capslock) {
		upper = TRUE;
	} else if (current.capslock && !current.shift && is_alpha_code(current.scancode)) {
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

static uint8_t get_ascii(keypress_t current, bool upper) {
	static unsigned char const codes[125][2] = {KEYCODES_TO_QWERTY};
	return (codes[current.keycode][upper]);
}

static keypress_t handle_control(keypress_t current) {
	if (current.pressed) {
		current.control = TRUE;
	} else {
		current.control = FALSE;
	}
	return (current);
}

static keypress_t handle_shift(keypress_t current) {
	if (current.pressed) {
		current.shift = TRUE;
	} else {
		current.shift = FALSE;
	}
	return (current);
}

static keypress_t handle_alt(keypress_t current) {
	if (current.pressed) {
		current.alt = TRUE;
	} else {
		current.alt = FALSE;
	}
	return (current);
}
static keypress_t handle_gui(keypress_t current) {
	if (current.pressed) {
		current.gui = TRUE;
	} else {
		current.gui = FALSE;
	}
	return (current);
}
static keypress_t handle_capslock(keypress_t current) {
	if (current.pressed) {
		current.capslock = !current.capslock;
	}
	return (current);
}

static keypress_t handle_numlock(keypress_t current) {
	if (current.pressed) {
		current.numlock = !current.numlock;
	}
	return (current);
}

static keypress_t handle_scrolllock(keypress_t current) {
	if (current.pressed) {
		current.scrolllock = !current.scrolllock;
	}
	return (current);
}

static void handle_fn(keypress_t* current) {
	if (current->scancode < 0x45) {
		current->keycode = current->scancode - 0x3A;
	} else {
		current->keycode = current->scancode - 0x4C;
	}
}
