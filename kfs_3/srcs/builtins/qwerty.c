#include "builtin.h"
#include "kernel.h"
#include "keycode.h"

uint8_t qwerty(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	keyboard_layout(LAYOUT_QWERTY);
	return (0);
}

uint8_t azerty(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	keyboard_layout(LAYOUT_AZERTY);
	return (0);
}
