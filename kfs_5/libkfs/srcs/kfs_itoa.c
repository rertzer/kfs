#include "kfs_itoa.h"
#include <stdio.h>

static char* recurse_itoa(int value, char* buffer, int radix);

char* kfs_itoa(int value, char* buffer, int radix) {
	recurse_itoa(value, buffer, radix);
	return (buffer);
}

char* recurse_itoa(int value, char* buffer, int radix) {
	static char alpha[] = "0123456789ABCDEF";

	int quotient = value / radix;
	if (quotient != 0) {
		buffer = kfs_itoa(quotient, buffer, radix);
		++buffer;
	}
	buffer[0] = '0' + value % radix;
	buffer[1] = '\0';
	printf("%s\n", buffer);
	return (buffer);
}
