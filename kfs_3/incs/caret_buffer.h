#ifndef CARET_BUFFER_H
#define CARET_BUFFER_H

#include <stdint.h>
#include "string.h"
#include "utils.h"

typedef struct s_caret_buffer {
    uint8_t *buffer;
    size_t  capacity;
    size_t  len;
    size_t  caret_position;
} t_caret_buffer;

void    caret_buffer_reset(t_caret_buffer* this);
int     caret_buffer_update_caret_position(t_caret_buffer* this, int n);

int     caret_buffer_select(t_caret_buffer* this, int n, uint8_t *dest, size_t dest_size);
int     caret_buffer_remove(t_caret_buffer* this, int n);
int     caret_buffer_insert(t_caret_buffer* this, uint8_t *src, size_t src_size);
int     caret_buffer_cut(t_caret_buffer* this, int n, uint8_t *dest, size_t dest_size);

#endif // CARET_BUFFER_H