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
	handle_fun_t handle_fun[256];

	for (size_t i = 0; i < 256; ++i) {
		handle_fun[i] = handle_ascii;
	}
	for (size_t i = 0x3B; i <= 0x44; ++i) {
		handle_fun[i] = handle_fn;
	}
	handle_fun[0x1D] = handle_control;
	handle_fun[0x2A] = handle_shift;
	handle_fun[0x36] = handle_shift;
	handle_fun[0x38] = handle_alt;
	handle_fun[0x3A] = handle_capslock;
	handle_fun[0x45] = handle_numlock;
	handle_fun[0x46] = handle_scrolllock;
	handle_fun[0x57] = handle_fn;
	handle_fun[0x58] = handle_fn;

	handle_fun[current->scancode](current);
}

static void handle_extended_code(keypress_t* current) {
	static const keycode_t codes[110] = {
		[0x10] = PREVIOUS_TRACK,
		[0x19] = NEXT_TRACK,
		[0x1C] = KEYPAD_ENTER,
		[0x1D] = R_CONTROL,
		[0x20] = MUTE,
		[0x21] = CALCULATOR,
		[0x22] = PLAY,
		[0x24] = STOP,
		[0x2E] = VOL_DOWN,
		[0x30] = VOL_UP,
		[0x32] = WWW_HOME,
		[0x35] = KEYPAD_SLASH,
		[0x38] = R_ALT,
		[0x47] = HOME,
		[0x48] = UP,
		[0x49] = PAGE_UP,
		[0x4B] = LEFT,
		[0x4D] = RIGHT,
		[0x4F] = END,
		[0x50] = DOWN,
		[0x51] = PAGE_DOWN,
		[0x52] = INSERT,
		[0x53] = DELETE,
		[0x5B] = L_GUI,
		[0x5C] = R_GUI,
		[0x5D] = APPS,
		[0x5E] = POWER,
		[0x5F] = SLEEP,
		[0x63] = WAKE,
		////////////////////////////////////////////////////////////////
		[0x65] = WWW_SEARCH,
		[0x66] = WWW_FAVORITE,
		[0x67] = WWW_REFRESH,
		[0x68] = WWW_STOP,
		[0x69] = WWW_FORWARD,
		[0x6A] = WWW_BACK,
		[0x6B] = MY_COMPUTER,
		[0x6C] = EMAIL,
		[0x6D] = MEDIA_SELECT};
	current->keycode = (uint8_t)codes[current->scancode];
	switch (current->keycode) {
		case R_CONTROL:
			handle_control(current);
			break;
		case R_ALT:
			handle_alt(current);
			break;
		case KEYPAD_ENTER:
			current->ascii = '\n';
			break;
		case KEYPAD_SLASH:
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
	static unsigned char const codes[84][2] = {
		[0x01] = {27, 27},	// escape
		[0x02] = {'1', '!'},
		[0x03] = {'2', '@'},
		[0x04] = {'3', '#'},
		[0x05] = {'4', '$'},
		[0x06] = {'5', '%'},
		[0x07] = {'6', '^'},
		[0x08] = {'7', '&'},
		[0x09] = {'8', '*'},
		[0x0A] = {'9', '('},
		[0x0B] = {'0', ')'},
		[0x0C] = {'-', '_'},
		[0x0D] = {'=', '+'},
		[0x0E] = {8, 8},  // backspace
		[0x0F] = {'\t', '\t'},
		[0x10] = {'q', 'Q'},
		[0x11] = {'w', 'W'},
		[0x12] = {'e', 'E'},
		[0x13] = {'r', 'R'},
		[0x14] = {'t', 'T'},
		[0x15] = {'y', 'Y'},
		[0x16] = {'u', 'U'},
		[0x17] = {'i', 'I'},
		[0x18] = {'o', 'O'},
		[0x19] = {'p', 'P'},
		[0x1A] = {'[', '{'},
		[0x1B] = {']', '}'},
		[0x1C] = {'\n', '\n'},
		// 0x1D left control
		[0x1E] = {'a', 'A'},
		[0x1F] = {'s', 'S'},
		[0x20] = {'d', 'D'},
		[0x21] = {'f', 'F'},
		[0x22] = {'g', 'G'},
		[0x23] = {'h', 'H'},
		[0x24] = {'j', 'J'},
		[0x25] = {'k', 'K'},
		[0x26] = {'l', 'L'},
		[0x27] = {';', ':'},
		[0x28] = {'\'', '\"'},
		[0x29] = {'`', '~'},
		// 0x2A left shift
		[0x2B] = {'\\', '|'},
		[0x2C] = {'z', 'Z'},
		[0x2D] = {'x', 'X'},
		[0x2E] = {'c', 'C'},
		[0x2F] = {'v', 'V'},
		[0x30] = {'b', 'B'},
		[0x31] = {'n', 'N'},
		[0x32] = {'m', 'M'},
		[0x33] = {',', '<'},
		[0x34] = {'.', '>'},
		[0x35] = {'/', '?'},
		// 0x36 right shift
		[0x37] = {'*', '*'},
		// 0x38 left alt
		[0x39] = {' ', ' '},
		// 0x3A capslock
		//  0x3B -> 0x44 Fx
		// 0x45 numlock
		// 0x46 scrolllock
		[0x47] = {'7', '7'},
		[0x48] = {'8', '8'},
		[0x49] = {'9', '9'},
		[0x4A] = {'-', '-'},
		[0x4B] = {'4', '4'},
		[0x4C] = {'5', '5'},
		[0x4D] = {'6', '6'},
		[0x4E] = {'+', '+'},
		[0x4F] = {'1', '1'},
		[0x50] = {'2', '2'},
		[0x51] = {'3', '3'},
		[0x52] = {'0', '0'},
		[0x53] = {'.', '.'},
	};
	current->ascii = codes[current->scancode][upper];
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
