#ifndef ECMA48_H
#define ECMA48_H

typedef void (*t_fp_ecma48_char_handler) (Type*, char);
typedef void (*t_fp_ecma48_empty_handler) (Type*);
typedef void (*t_fp_ecma48_cursor_handler) (Type*, Vec2);
typedef void (*t_fp_ecma48_scroll_handler) (Type*, uint);
typedef void (*t_fp_ecma48_handler) (Type*, uint...);

typedef struct s_ecma48_handlers {
    t_fp_ecma48_cursor_handler      on_cursor_mouvement;
    t_fp_ecma48_cursor_handler      on_set_cursor_position;

    t_fp_ecma48_scroll_handler      on_scroll_up;
    t_fp_ecma48_scroll_handler      on_scroll_down;

    t_fp_ecma48_handler             on_graphic_rendition;

    t_fp_ecma48_empty_handler       on_clear_screen;
    t_fp_ecma48_empty_handler       on_next_line;
    t_fp_ecma48_char_handler        default_char_handler;
    t_fp_ecma48_char_handler        char_handlers[255];
} t_ecma48_handlers;

#endif // ECMA48_H
