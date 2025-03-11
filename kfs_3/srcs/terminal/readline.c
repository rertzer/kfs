#include "ecma48.h"

#include "readine.h"

#include <stdint.h>
#include "string.h"

typedef struct s_readline_buffer {
    uint8_t *buffer;
    size_t  capacity;
    size_t  caret_position;
    size_t  position;
} t_readline_buffer;

void reset(t_readline_buffer* this) {
    memset(this->buffer, 0. this->capacity);
    this->capacity = 0;
}

int update_caret_position(t_readline_buffer* this, int n) {
    size_t  remaining;
    
    remaining = this->capacity - this->caret_position;
    if (n > 0 && n > remaining) {
        n = remaining;
    } else if (n < 0 && -n > this->caret_position) {
        n = -this->caret_position;
    }
    this->caret_position += n;
    return (n);
}

int select(t_readline_buffer* this, int n, uint8_t *dest, size_t dest_size)
{
    if (n == 0) {
        return 0;
    }
    int len = math::abs(n);
    if (len > buffer.len) {
        return Err.UNABLE_TO_SELECT?;
    }

    int max = this.caret_position;
    if (n > 0) max = this.buffer.size - max;
    
    clampe_value::clamp(<int>)(0, max, &len);
    
    Type* source = &this.buffer.data[this.caret_position];
    if (n < 0) source -= len;
    memory::copy(<Type>)(buffer, source, len);

    return (n < 0) ? -len : len;
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


void readline(t_readline_buffer* this) {
    hooks_fd(&ecma48_handlers, this ,STD_IN);
}

#define TAB_SIZE 4

int! CaretBuffer.select(CaretBuffer* this, Type[] buffer, int n = 0)
{
    if (n == 0) return 0;
    int len = math::abs(n);
    if (len > buffer.len) return Err.UNABLE_TO_SELECT?;

    int max = this.caret_position;
    if (n > 0) max = this.buffer.size - max;
    
    clampe_value::clamp(<int>)(0, max, &len);
    
    Type* source = &this.buffer.data[this.caret_position];
    if (n < 0) source -= len;
    memory::copy(<Type>)(buffer, source, len);

    return (n < 0) ? -len : len;
}

int CaretBuffer.remove(CaretBuffer* this, int n = 0)
{
    if (n > this.buffer.size) return 0;
    if (this.caret_position.can_be_updated_by(n) == false) return 0;
    this.buffer.remove(this.caret_position.get_range_relative_to(n));
    if (n < 0) this.caret_position.update(n);
    return (n);
}

int CaretBuffer.insert(CaretBuffer* this, Type[] str)
{
    return this.caret_position.update(this.buffer.insert(this.caret_position, str));
}










module readline;

import utils::buffer;
import utils;
import utils::string;
import std_io;

const uint READLINE_BUFFER_SIZE = 1024;

struct Readline
{
    inline CaretBuffer(<char, READLINE_BUFFER_SIZE>) b;
}

fn void Readline.readline(Readline* this)
{
    readline::handler::HANDLER.hooks_fd(this, std_io::STD_IN);
}

module readline::handler;

import utils::vector;
import utils::window;
import utils::string;
import utils::math;
import std_io;
import ecma48;

const int FD = std_io::STD_OUT;

const Ecma48_handlers(<Readline>) HANDLER = {
    .on_cursor_mouvement    = &Readline.on_cursor_mouvement,
    .default_char_handler   = &Readline.on_default,
    .char_handlers = {
        [0x08]              = &Readline.on_backspace,
        [0x7F]              = &Readline.on_delete,
        [0x0A]              = &Readline.on_enter,
    }
};

const int	TABSIZE = 4;

fn void Readline.on_backspace(Readline* this, char c)
{
    char[1] tmp;
    this.b.select(&tmp, -1)!!;
    if(!this.b.remove(-1)) return;

    int len = (tmp[0] == '\t') ? TABSIZE : 1;
    while (len--) std_io::write(FD, char[]{0x08}, 1);
}

fn void Readline.on_delete(Readline* this, char c)
{
    char[1] tmp;
    this.b.select(&tmp, 1)!!;
    if(!this.b.remove(1)) return;

    int len = (tmp[0] == '\t') ? TABSIZE : 1;
    while (len--) std_io::write(FD, char[]{0x7F}, 1);
}

fn void Readline.on_default(Readline* this, char c)
{
    if (!this.b.insert({c})) return;

    int len = (c == '\t') ? TABSIZE : 1;
    c = (c == '\t') ? ' ' : c;

    while (len--) std_io::write(FD, char[]{c}, 1);
}

fn void Readline.on_enter(Readline* this, char c)
{
    this.b.insert({'\n'}); // Will be removed or not
    std_io::write(FD, char[]{0x0A}, 1);
}

fn void Readline.on_cursor_mouvement(Readline* this, Vec2 mouvement)
{
    char[1] tmp;
    int len = math::abs(mouvement.x);
    int dir = (mouvement.x < 0) ? -1 : 1;

    while (len--) {
        this.b.select(&tmp, dir)!!;
        if(!this.b.update_caret(dir)) return;
        int direction = (tmp[0] == '\t') ? (TABSIZE * dir) : dir;
        ecma48::handler::move_cursor(FD, Vec2{direction, 0});
    }
}