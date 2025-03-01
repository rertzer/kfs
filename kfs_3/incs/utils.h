#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* string.c */
size_t  strlen(const char* str);

size_t  ft_strrchr(const char* str, char c);

int     ft_memcmp(void* s1, void* s2, size_t n);
void    ft_memset(void* address, char c, uint32_t size);
void*	ft_memcpy(void* dest, const void* src, size_t n);

/* ctype.c */
bool    is_space(uint8_t c);
bool    is_digit(uint8_t c);
bool    is_upper(uint8_t c);
bool    is_lower(uint8_t c);
bool    is_alpha(uint8_t c);
bool    is_alnum(uint8_t c);
int     to_upper(int c);
int     to_lower(int c);

/* stdlib.c */
size_t  ft_atoi(const char* str);

/* math.c */
int32_t min(int32_t a, int32_t b);
int32_t max(int32_t a, int32_t b);
uint32_t abs(int32_t v);


#endif // UTILS_H
