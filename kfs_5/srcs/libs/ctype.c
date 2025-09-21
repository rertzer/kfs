#include "ctype.h"
#include "utils.h"

bool is_alpha(uint8_t c) {
	return (islower(c) || isupper(c));
}

bool is_alnum(uint8_t c) {
	return (isdigit(c) || is_alpha(c));
}

int to_upper(int c) {
	return (islower(c) ? (c - 0x20) : c);
}

int to_lower(int c) {
	return (isupper(c) ? (c + 0x20) : c);
}
