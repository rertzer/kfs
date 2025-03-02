#ifndef UNISTD_H
#define UNISTD_H

#include "terminal.h"

#define STD_IN  0
#define STD_OUT 1

int read(int fd, uint8_t* buffer, size_t buffer_size);
int	write(int fd, char* buffer, size_t buffer_size);

#endif // UNISTD_H