#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>

typedef struct s_tty {
    struct {
        uint16_t        *buffer;
        const size_t    height;
        const size_t    width;
    } frame;
    struct {
        size_t  x;
        size_t  y;
    } cursor;
    
	uint8_t	  color;

	// size_t	  row;
	// size_t	  column;

	// size_t	  prompt_row;
	// size_t	  prompt_column;

	// size_t	  line_len;
	// size_t	  left_margin;
} t_tty;


#endif // TTY_H