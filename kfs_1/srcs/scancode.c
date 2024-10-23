#include "kernel.h"

static void		   handle_pause(keypress_t* current, uint8_t scancode);
static void		   handle_extended(keypress_t* current, uint8_t scancode);
static void		   handle_pressed(keypress_t* current, uint8_t scancode);
static void		   handle_code(keypress_t* current);
static void		   handle_extended_code(keypress_t* current);
static inline bool is_alpha_code(uint8_t scancode);
static void		   handle_ascii(keypress_t* current);
static bool		   get_upper(keypress_t* current);
static void		   set_ascii(keypress_t* current, bool upper);
static void		   handle_fn(keypress_t* current);

keypress_t handle_scancode(uint8_t scancode) {
	static keypress_t current;

	current.ascii = 0;
	if (current.keycode > 0xF9) {
		handle_pause(&current, scancode);
	} else if (current.extended != FALSE) {
		handle_extended(&current, scancode);
	} else {
		handle_pressed(&current, scancode);
		handle_code(&current);
	}
	return (current);
}

static void handle_pause(keypress_t* current, uint8_t scancode) {
	/* !!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!!!!! */
	current->scancode = scancode;
}

static void handle_extended(keypress_t* current, uint8_t scancode) {
	handle_pressed(current, scancode);
	handle_extended_code(current);
}

static void handle_pressed(keypress_t* current, uint8_t scancode) {
	current->pressed = FALSE;
	current->ascii = 0;
	current->keycode = 0;
	if (scancode < 0x80) {
		current->pressed = TRUE;
		current->scancode = scancode;
	} else if (scancode < 0xE0) {
		current->scancode = scancode - 0x80;
	} else if (scancode == 0xE0) {
		current->extended = TRUE;
		current->scancode = 0;
	} else if (scancode == 0xE1) {
		current->keycode = 0xFA;
		current->scancode = 0;
	}
}

static void handle_code(keypress_t* current) {
	static const keycode_t codes[128] = {SCANCODES};
	current->keycode = (uint8_t)codes[current->scancode];
	handle_ascii(current);
	// handle_fun_t handle_fun[256];

	//
	// for (size_t i = 0; i < 256; ++i) {
	// 	handle_fun[i] = handle_ascii;
	// }
	// for (size_t i = 0x3B; i <= 0x44; ++i) {
	// 	handle_fun[i] = handle_fn;
	// }
	// handle_fun[0x1D] = handle_control;
	// handle_fun[0x2A] = handle_shift;
	// handle_fun[0x36] = handle_shift;
	// handle_fun[0x38] = handle_alt;
	// handle_fun[0x3A] = handle_capslock;
	// handle_fun[0x45] = handle_numlock;
	// handle_fun[0x46] = handle_scrolllock;
	// handle_fun[0x57] = handle_fn;
	// handle_fun[0x58] = handle_fn;
	//
	// handle_fun[current->scancode](current);
}

static void handle_extended_code(keypress_t* current) {
	static const keycode_t codes[128] = {EXTENDED_SCANCODES};

	current->keycode = (uint8_t)codes[current->scancode];
	switch (current->keycode) {
		case R_CONTROL:
			handle_control(current);
			break;
		case R_ALT:
			handle_alt(current);
			break;
		case PAD_ENTER:
			current->ascii = '\n';
			break;
		case PAD_SLASH:
			current->ascii = '/';
			break;
		default:
			break;
	}
	current->extended = FALSE;
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
