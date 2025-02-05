#include <kernel.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern volatile uint8_t current_code;

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) {
		++len;
	}
	return (len);
}

bool is_alnum(uint8_t c) {
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
		return (true);
	} else {
		return (false);
	}
}

void ft_memset(uint8_t* address, char c, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i) {
		address[i] = c;
	}
}

uint32_t round_up_power_two(uint32_t n) {
	uint32_t offset = 31;
	if (n == 0) {
		return (0);
	}
	for (; offset != 0; --offset) {
		if ((n >> offset) == 1) {
			break;
		}
	}
	if ((n - (1 << offset)) != 0) {
		++offset;
	}
	return (offset);
}

void press_any() {
	keypress_t keypress = init_keypress();

	printk("press any key to continue\n");

	while (true) {
		keypress.keycode = current_code;
		current_code = 0;
		if (keypress.keycode != 0) {
			keypress = update_keypress(keypress);

			if (keypress.pressed == PRESSED) {
				break;
			}
		}
		sleep();
	}
}
