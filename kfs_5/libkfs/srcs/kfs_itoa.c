#include "kfs_itoa.h"
#include <stdbool.h>
#include <stddef.h>

static inline bool		   invalid_radix(int radix);
static char*			   recurse_itoa(unsigned int value, char* buffer, int radix);
static inline unsigned int set_sign(int value, char** buffer, int radix);
static inline void		   null_terminate(char* buffer);

char* kfs_itoa(int value, char* buffer, int radix) {
	unsigned int uvalue;
	char*		 buffer_start = buffer;

	if (invalid_radix(radix)) {
		return (NULL);
	}

	uvalue = set_sign(value, &buffer, radix);
	buffer = recurse_itoa(uvalue, buffer, radix);
	null_terminate(buffer);
	return (buffer_start);
}

inline bool invalid_radix(int radix) {
	return (radix < 2 || radix > 36);
}

inline unsigned int set_sign(int value, char** buffer, int radix) {
	unsigned int uvalue;
	if (radix == 10 && value < 0) {
		*buffer[0] = '-';
		++*buffer;
		uvalue = -value;

	} else {
		uvalue = value;
	}
	return (uvalue);
}

char* recurse_itoa(unsigned int uvalue, char* buffer, int radix) {
	static char alpha[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	int quotient = uvalue / radix;
	if (quotient != 0) {
		buffer = recurse_itoa(quotient, buffer, radix);
	}
	buffer[0] = alpha[uvalue % radix];
	return (buffer + 1);
}

inline void null_terminate(char* buffer) {
	buffer[0] = '\0';
}
