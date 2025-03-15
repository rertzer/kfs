#include "utils.h"

int32_t min(int32_t a, int32_t b) {
	return (a < b ? a : b);
}

int32_t max(int32_t a, int32_t b) {
	return (a > b ? a : b);
}

uint32_t abs(int32_t v) {
	return (v >= 0 ? v : -v);
}

inline int32_t clamp(int32_t value, int32_t min, int32_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}