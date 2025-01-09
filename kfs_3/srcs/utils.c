#include <kernel.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

void page_testing() {
	char* good_string = "hello word\n";
	char* fake_addr = (char*)0xD09DC300;
	printk("%s\n", good_string);
	printk("my funny addr %s\n", fake_addr);
}

void ft_memset(char* address, char c, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i) {
		address[i] = c;
		if (i == 2) {
			godot();
		}
	}
}
