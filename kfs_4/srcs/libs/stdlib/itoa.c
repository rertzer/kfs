#include "stdlib.h"

static const char *base = "0123456789abcdefghijklmnopqrstuvwxyz";

char* itoa (int val, char * buffer, int radix) {
    int next = val / radix;
    if (next) buffer = itoa(next, buffer, radix) + 1;
    buffer[0] = base[val % radix];
    return (buffer);
} // TODO: Taking care of itoa because the returned string is not ok
