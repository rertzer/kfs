#include "unistd.h"

int	write(int fd, char* buffer, size_t buffer_size) {
	(void)fd;
	return (term_putchars(buffer, buffer_size));
}