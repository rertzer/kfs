#include "utils.h"

bool is_digit(uint8_t c) {
	return (c >= '0' && c <= '9');
}

bool is_upper(uint8_t c) {
	return (c >= 'A' && c <= 'Z');
}

bool is_lower(uint8_t c) {
	return (c >= 'a' && c <= 'z');
}

bool is_alpha(uint8_t c) {
	return (is_lower(c) || is_upper(c));
}

bool is_alnum(uint8_t c) {
	return (is_digit(c) || is_alpha(c));
}

int to_upper(int c) {
	return (is_lower(c) ? (c - 0x20) : c);
}

int to_lower(int c) {
	return (is_upper(c) ? (c + 0x20) : c);
}
