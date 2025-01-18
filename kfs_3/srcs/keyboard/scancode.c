#include "kernel.h"
#include "keycode.h"

static bool	   handle_pressed();
static uint8_t handle_status();
static void	   handle_code(bool pressed);
static void	   handle_extended_code(bool pressed);

uint8_t	  current_scancode = 0;
keycode_t current_code = NONE;

void keyboard_handler(uint8_t scan) {
	static uint8_t scan_status = SCAN_DEFAULT;

	current_scancode = scan;

	switch (scan_status) {
		case SCAN_DEFAULT:
			scan_status = handle_scancode_default();
			break;
		case SCAN_EXTENDED:
			scan_status = handle_scancode_extended();
			break;
		case SCAN_PAUSE:
			scan_status = handle_scancode_pause();
			break;
	}
	current_scancode = 0;
}

uint8_t handle_scancode_pause() {
	static uint8_t pause_sequence[5] = {0x1D, 0x45, 0xE1, 0x9D, 0xc5};
	static size_t  index;

	uint8_t scan_status = SCAN_PAUSE;
	if (current_scancode == pause_sequence[index]) {
		++index;
		if (index == 5) {
			current_code = PAUSE;
			index = 0;
			scan_status = SCAN_DEFAULT;
		}
	} else {
		index = 0;
		scan_status = SCAN_DEFAULT;
	}

	return (scan_status);
}

uint8_t handle_scancode_default() {
	bool	pressed = handle_pressed();
	uint8_t status = handle_status();
	if (status == SCAN_DEFAULT) {
		handle_code(pressed);
	} else {
		current_code = NONE;
	}
	return (status);
}

uint8_t handle_scancode_extended() {
	bool pressed = handle_pressed();
	handle_extended_code(pressed);
	return (SCAN_DEFAULT);
}

static bool handle_pressed() {
	bool pressed = RELEASED;

	if (current_scancode < 0x80) {
		pressed = PRESSED;
	} else if (current_scancode < 0xE0) {
		current_scancode -= 0x80;
	}
	return (pressed);
}

static uint8_t handle_status() {
	uint8_t status = SCAN_DEFAULT;

	if (current_scancode == 0xE0) {
		status = SCAN_EXTENDED;
	} else if (current_scancode == 0xE1) {
		status = SCAN_PAUSE;
	}
	return (status);
}

static void handle_code(bool pressed) {
	static const keycode_t codes[128] = {SCANCODES};
	keycode_t			   kc = codes[current_scancode];
	current_code = (pressed << 7) | (uint8_t)kc;
}

static void handle_extended_code(bool pressed) {
	static const keycode_t codes[128] = {EXTENDED_SCANCODES};
	keycode_t			   kc = codes[current_scancode];
	current_code = (pressed << 7) | (uint8_t)kc;
}
