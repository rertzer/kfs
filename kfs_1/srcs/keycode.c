#include "kernel.h"

static uint8_t	   numlock_converter(keypress_t keypress);
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
	keypress.pressed = RELEASED;
	keypress.control = false;
	keypress.alt = false;
	keypress.shift = false;
	keypress.gui = false;
	keypress.capslock = false;
	keypress.numlock = true;
	keypress.scrolllock = false;
	keypress.scancode = 0;
	keypress.ascii = 0;
	keypress.keycode = 0;

	return (keypress);
}

keypress_t update_keypress(keypress_t keypress) {
	keypress.pressed = keypress.keycode >> 7;
	keypress.keycode = keypress.keycode & 0x7F;
	keypress.keycode = numlock_converter(keypress);
	keypress.ascii = handle_ascii(keypress);
	keypress = handle_switches(keypress);
	return (keypress);
}

static uint8_t numlock_converter(keypress_t keypress) {
	uint8_t keycode;
	uint8_t non_num_pad[16] = {NONNUMPAD};
	if (keypress.numlock == false && keypress.keycode >= PAD_1) {
		keycode = non_num_pad[keypress.keycode - PAD_1];
	} else {
		keycode = keypress.keycode;
	}
	return (keycode);
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
////////////////////////////////////////////////////////
static uint8_t handle_ascii(keypress_t current) {
	bool upper = get_upper(current);
	return (get_ascii(current, upper));
}

static bool get_upper(keypress_t current) {
	bool upper = FALSE;
	if (current.shift && !current.capslock) {
		upper = TRUE;
	} else if (current.capslock && !current.shift && is_alpha_code(current.keycode)) {
		upper = TRUE;
	}
	return (upper);
}

static inline bool is_alpha_code(uint8_t keycode) {
	bool is = FALSE;

	if ((keycode >= 0x10 && keycode <= 0x19) || (keycode >= 0x1F && keycode <= 0x27) ||
		(keycode >= 0x2C && keycode <= 0x33)) {
		is = TRUE;
	}
	return (is);
}

static uint8_t get_ascii(keypress_t current, bool upper) {
	static unsigned char const codes[126][2] = {KEYCODES_TO_QWERTY};
	return (codes[current.keycode][upper]);
}

static keypress_t handle_control(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.control = TRUE;
	} else {
		current.control = FALSE;
	}
	return (current);
}

static keypress_t handle_shift(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.shift = TRUE;
	} else {
		current.shift = FALSE;
	}
	return (current);
}

static keypress_t handle_alt(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.alt = TRUE;
	} else {
		current.alt = FALSE;
	}
	return (current);
}
static keypress_t handle_gui(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.gui = TRUE;
	} else {
		current.gui = FALSE;
	}
	return (current);
}
static keypress_t handle_capslock(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.capslock = !current.capslock;
	}

	return (current);
}

static keypress_t handle_numlock(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.numlock = !current.numlock;
	}
	return (current);
}

static keypress_t handle_scrolllock(keypress_t current) {
	if (current.pressed == PRESSED) {
		current.scrolllock = !current.scrolllock;
	}
	return (current);
}

static void handle_fn(keypress_t* current) {
	if (current->keycode < 0x45) {
		current->keycode = current->keycode - 0x3A;
	} else {
		current->keycode = current->keycode - 0x4C;
	}
}
