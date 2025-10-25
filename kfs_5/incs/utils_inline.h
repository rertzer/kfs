#ifndef UTILS_INLINE_H
#define UTILS_INLINE_H

#include <stdint.h>

inline int get_bit(uint32_t field, uint32_t index) {
	return ((field & ((uint32_t)1 << index)) >> index);
}

inline uint32_t set_bit(uint32_t field, uint32_t index) {
	return (field | (uint32_t)1 << index);
}

inline uint32_t unset_bit(uint32_t field, uint32_t index) {
	return (field & ~((uint32_t)1 << index));
}

#endif
