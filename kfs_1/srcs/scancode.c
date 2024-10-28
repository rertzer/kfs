#include "kernel.h"
#include "keycode.h"

static bool	   handle_pressed();
static uint8_t handle_status();
static void	   handle_code(bool pressed);
static void	   handle_extended_code(bool pressed);
extern uint8_t current_scancode;

uint8_t handle_scancode_pause() {
	/* !!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!!!!! */
	current_scancode = PAUSE;
	return (SCAN_DEFAULT);
}

uint8_t handle_scancode_default() {
	bool	pressed = handle_pressed();
	uint8_t status = handle_status();
	handle_code(pressed);
	return (status);
}

uint8_t handle_scancode_extended() {
	bool pressed = handle_pressed();
	handle_extended_code(pressed);
	return (SCAN_DEFAULT);
}

static bool handle_pressed() {
	bool pressed = false;
	if (current_scancode < 0x80) {
		pressed = true;
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
	current_scancode = (pressed << 7) | (uint8_t)kc;
}

static void handle_extended_code(bool pressed) {
	static const keycode_t codes[128] = {EXTENDED_SCANCODES};
	keycode_t			   kc = codes[current_scancode];
	current_scancode = (pressed << 7) | (uint8_t)kc;
}
