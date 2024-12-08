#include "builtin.h"
#include "kernel.h"
#include "keycode.h"

uint8_t qwerty() {
	uint32_t bb = boom();
	printk("bibi %08x\n", bb);
	keyboard_layout(LAYOUT_QWERTY);
	return (0);
}

uint8_t azerty() {
	keyboard_layout(LAYOUT_AZERTY);
	return (0);
}
