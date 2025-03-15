#include "ecma48.h"

#include "readine.h"
#include "caret_buffer.h"



#define FD std_io::STD_OUT
#define	TABSIZE 4

typedef struct s_readline {
    t_caret_buffer  buffer;
} t_readline;

static void    on_backspace(void* self, char c) {
    t_readline  *this = self;
    char        tmp[1] = {0};

    if (!caret_buffer_cut(this->buffer, -1, &tmp, 1)) {
        return;
    }
    int len = (tmp[0] == '\t') ? TABSIZE : 1;
    while (len--) {
        write(FD, 0x08, 1);
    }
}

static void    on_delete(void* self, char c) {
    t_readline  *this = self;
    char tmp[1] = {0};

    if (!caret_buffer_cut(this->buffer, 1, &tmp, 1)) {
        return;
    }
    int len = (tmp[0] == '\t') ? TABSIZE : 1;
    while (len--) {
        write(FD, 0x7F, 1);
    }
}

static void    on_default(void* self, char c) {
    t_readline  *this = self;
    char        tmp[1] = {c};

    if (!caret_buffer_insert(this->buffer, tmp, 1)) {
        return;
    }
    int len = (c == '\t') ? TABSIZE : 1;
    c = (c == '\t') ? ' ' : c;
    while (len--) {
        write(FD, tmp, 1);
    }
}

static void    on_enter(void* self, char c) {
    t_readline  *this = self;
    char        tmp[1] = {'\n'};

    if (!caret_buffer_insert(this->buffer, tmp, 1)) {
        return;
    }
    write(FD, tmp, 1);
}

static void    on_cursor_mouvement(void* self, int32_t x, int32_t y) {
    t_readline  *this = self;
    char        tmp[1];
    int         len = abs(x);
    int         dir = (x < 0) ? -1 : 1;

    while (len--) {
        caret_buffer_select(this->buffer, dir, tmp, 1);
        if (!caret_buffer_update_caret_position(this->buffer, dir)) {
            return;
        }
        int direction = (tmp[0] == '\t') ? (TABSIZE * dir) : dir;
        ecma48::handler::move_cursor(FD, direction, 0);
    }
}

static const t_ecma48_handlers ecma48_handlers = {
    .on_cursor_mouvement    = on_cursor_mouvement,
    .default_char_handler   = on_default,
    .char_handlers = {
        [0x08]              = on_backspace,
        [0x7F]              = on_delete,
        [0x0A]              = on_enter,
    }
};

void readline(t_readline* this) {
    hooks_fd(&ecma48_handlers, this ,STD_IN);
}