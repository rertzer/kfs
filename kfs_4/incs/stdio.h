#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#ifndef PRINTF_BUFFER_SIZE
# define PRINTF_BUFFER_SIZE 2048
#endif

int	vsprintf(char* buffer, const char* format, va_list l_args);    
int vdprintf(int fd, const char* format, va_list l_args);        
int dprintf(int fd, const char* format, ...);            
int	sprintf(char* buffer, const char *format, ...);                
int	printf(const char *format, ...);

#include "character_set.h" // TODO: Only to facilitate access across files

#endif // STDIO_H