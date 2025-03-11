#ifndef ECMA48_H
#define ECMA48_H

#include <stdint.h>

typedef void (*t_fp_ecma48_char_handler) (void*, char);
typedef void (*t_fp_ecma48_empty_handler) (void*);
typedef void (*t_fp_ecma48_cursor_handler) (void*, int32_t, int32_t);
typedef void (*t_fp_ecma48_scroll_handler) (void*, int32_t);
// typedef void (*t_fp_ecma48_handler) (void*, uint...);

typedef struct s_ecma48_handlers {
    t_fp_ecma48_cursor_handler      on_cursor_mouvement;
    t_fp_ecma48_cursor_handler      on_set_cursor_position;

    t_fp_ecma48_scroll_handler      on_scroll_up;
    t_fp_ecma48_scroll_handler      on_scroll_down;

    // t_fp_ecma48_handler             on_graphic_rendition;

    t_fp_ecma48_empty_handler       on_clear_screen;
    t_fp_ecma48_empty_handler       on_next_line;

    t_fp_ecma48_char_handler        default_char_handler;
    t_fp_ecma48_char_handler        char_handlers[255];
} t_ecma48_handlers;


void hooks(t_ecma48_handlers* this, void* ref, char* buffer, size_t size);
void hooks_fd(t_ecma48_handlers* this, void* ref, int fd);


#endif // ECMA48_H
