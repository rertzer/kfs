#include "caret_buffer.h"

void    caret_buffer_reset(t_caret_buffer* this) {
    memset(this->buffer, 0, this->capacity);
    this->caret_position = 0;
}

int     caret_buffer_update_caret_position(t_caret_buffer* this, int n) {
    int len;

    len = abs(n);
    if (n < 0) len = clamp(len, 0, this->caret_position);
    if (n > 0) len = clamp(len, this->caret_position, this->capacity);

    len = (n < 0) ? -len : len;
    this->caret_position += len;
    return (len);
}

int     caret_buffer_select(t_caret_buffer* this, int n, uint8_t *dest, size_t dest_size) {
    int     len;
    uint8_t *src;
    
    len = abs(n);
    if (n < 0) len = clamp(len, 0, this->caret_position);
    if (n > 0) len = clamp(len, this->caret_position, this->capacity);

    len = clamp(len, 0, dest_size);
    src = &this->buffer[this->caret_position];
    src += (n < 0) ? -len : 0;
    memcpy(dest, src, len);
    return ((n < 0) ? -len : len);
}

int     caret_buffer_remove(t_caret_buffer* this, int n) {
    int len;

    len = abs(n);
    if (n < 0) len = clamp(len, 0, this->caret_position);
    if (n > 0) len = clamp(len, this->caret_position, this->capacity);

    if (n < 0) update_caret_position(this, -len);

    memmove(
        &this->buffer[this->caret_position],
        &this->buffer[this->caret_position + len], 
        this->capacity - (this->caret_position + len));
    this->len -= len;
    memset(&this->buffer[this->len], 0, len);
    return ((n < 0) ? -len : len);
}

int     caret_buffer_insert(t_caret_buffer* this, uint8_t *src, size_t src_size) {
    src_size = clamp(src_size, 0, this->capacity - this->caret_position);
    
    memmove(
        &this->buffer[this->caret_position + src_size], 
        &this->buffer[this->caret_position],
        this->capacity - (this->caret_position + src_size));
    this->len += src_size;
    memcpy(&this->buffer[this->caret_position], src, src_size);
    return (update_caret_position(this, src_size));
}

int     caret_buffer_cut(t_caret_buffer* this, int n, uint8_t *dest, size_t dest_size) {
    n = caret_buffer_select(this->buffer, n, dest, dest_size);
    return (caret_buffer_remove(this->buffer, n));
}