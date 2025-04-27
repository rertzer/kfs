#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t argc;
	char** argv;
} arg_t;

/* string.c */
size_t strlen(const char* str);

size_t ft_strrchr(const char* str, char c);

int	  ft_memcmp(void* s1, void* s2, size_t n);
void  ft_memset(void* address, char c, uint32_t size);
void* ft_memcpy(void* dest, const void* src, size_t n);
int     strncmp(const char *s1, const char *s2, size_t n);


/* ctype.c */
bool is_space(uint8_t c);
bool is_digit(uint8_t c);
bool is_upper(uint8_t c);
bool is_lower(uint8_t c);
bool is_alpha(uint8_t c);
bool is_alnum(uint8_t c);
int	 to_upper(int c);
int	 to_lower(int c);

/* stdlib.c */
size_t ft_atoi(const char* str);

/* math.c */
int32_t	 min(int32_t a, int32_t b);
int32_t	 max(int32_t a, int32_t b);
uint32_t abs(int32_t v);

/* jros */
arg_t arg_split(char const* const str);
void  arg_split_clean(arg_t* args);
bool  arg_split_test();
char* jr_substr(char const* const src, size_t start, size_t len);
char* term_substr(uint16_t const* const src, size_t start, size_t len);

#endif	// UTILS_H
