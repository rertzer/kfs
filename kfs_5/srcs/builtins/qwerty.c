#include "builtin.h"
#include "keycode.h"

uint8_t qwerty(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	keyboard_layout(LAYOUT_QWERTY);
	return (0);
}

uint8_t azerty(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	keyboard_layout(LAYOUT_AZERTY);
	return (0);
}
