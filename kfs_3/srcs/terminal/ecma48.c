#include "utils.h"

#define STD_IO_BUFFER_SIZE  128
int read(int fd, uint8_t* buffer, size_t buffer_size);


#define ESCAPE_SEQUENCE             '\e'
#define CONTROL_SEQUENCE_INTRODUCER 0x5B

typedef int Type; // TODO: remove it
typedef int Vec2; // TODO: remove it

typedef void (*t_fp_ecma48_char_handler)(Type*, char);
typedef void (*t_p_ecma48_empty_handler)(Type*);
typedef void (*t_fp_ecma48_cursor_handler)(Type*, Vec2);
typedef void (*t_fp_ecma48_scroll_handler)(Type*, int);
// typedef void (*t_fp_ecma48_handler)(Type*, int...); OLD
typedef void (*t_fp_ecma48_handler)(Type*, ...);

typedef struct s_t_ecma48_handlers {
    t_fp_ecma48_cursor_handler  on_cursor_mouvement;
    t_fp_ecma48_cursor_handler  on_set_cursor_position;

    t_fp_ecma48_scroll_handler  on_scroll_up;
    t_fp_ecma48_scroll_handler  on_scroll_down;

    t_fp_ecma48_handler         on_graphic_rendition;

    t_p_ecma48_empty_handler    on_clear_screen;
    t_p_ecma48_empty_handler    on_next_line;
    t_fp_ecma48_char_handler    default_char_handler;
    t_fp_ecma48_char_handler    char_handlers[255];
} t_ecma48_handlers;

int parse_input_parameter(char* input, int* output[16])
{
    int index = 0;
    int output_index = 0;

    while (input[index] && output_index < 16) {
        if(input[index] == ';') {
            index += 1;
            output_index += 1;
            continue;
        }
        if (is_digit(input[index])) {
            index += string::basic_atoi(&output[output_index], &input[index]);
            continue;
        }
        return index;
    }
    return index;
}

int dispatch(t_ecma48_handlers *this, Type* ref, char* input)
{
    int index = 0;

    if (input[index] == ESCAPE_SEQUENCE) {
        index += 1;
        switch (input[index])
        {
            case CONTROL_SEQUENCE_INTRODUCER:
            {
                index += 1;
                int params[16];
                index += parse_input_parameter(&input[index], &params);
                switch (input[index])
                {
                case 'A':
                    this->on_cursor_mouvement(ref, Vec2{0, -params[0]});
                    break;
                case 'B':
                    this->on_cursor_mouvement(ref, Vec2{0, +params[0]});
                    break;
                case 'C':
                    this->on_cursor_mouvement(ref, Vec2{+params[0], 0});
                    break;
                case 'D':
                    this->on_cursor_mouvement(ref, Vec2{-params[0], 0});
                    break;
                case 'H':
                    if (this->on_set_cursor_position) {
                        this->on_set_cursor_position(ref, Vec2{params[1], params[0]});
                    }
                    break;
                case 'f':
                    if (this->on_cursor_mouvement) {
                        this->on_cursor_mouvement(ref, Vec2{params[1], params[0]});
                    }
                    break;
                case 'S': if (this.on_scroll_up)            this.on_scroll_up(ref, params[0]);          break;
                case 'T': if (this.on_scroll_down)          this.on_scroll_down(ref, params[0]);        break;
                case 'm': if (this.on_graphic_rendition)    this.on_graphic_rendition(ref, params[0]);  break;
                case 'J':
                    if (params[0] == 2 && this->on_clear_screen) {
                        this->on_clear_screen(ref);
                    }
                    break;
                }
                return index + 1;
            }
            case 'E':
                if (this->on_next_line) {
                    this->on_next_line(ref);
                }
                break;
        }
    }

    char c = input[index];

    t_fp_ecma48_char_handler char_handler = this->char_handlers[c];
    if (char_handler == NULL) {
        char_handler = this->default_char_handler;
    }
    if (char_handler != NULL) {
        char_handler(ref, c);
    }
    return 1;
}

void hooks(t_ecma48_handlers* this, Type* ref, char* buffer, int size) {
    int index = 0;
    while (index < size) {
        index += dispatch(this, ref, &buffer[index]);
    }
}

void hooks_fd(t_ecma48_handlers* this, Type* ref, int fd) {
	int     read_size = 0;
    char    read_buffer[STD_IO_BUFFER_SIZE] = {};
    
    read_size = read(fd, &read_buffer, STD_IO_BUFFER_SIZE);
	if (read_size > 0) {
        hooks(this, ref, &read_buffer, read_size);
    }
}

module ecma48::handler;

import utils::string;
import utils::vector;
import keyboard::scancode;
import keyboard::codepage347;

void move_cursor(int fd, Vec2 mouvement) {
    int x = mouvement[0];
    int y = mouvement[1];
    
    if (y) {
        string::dprintf(fd, "\e[%d%c", abs(y), (y < 0) ? 'A' : 'B');
    }
    if (x) {
        string::dprintf(fd, "\e[%d%c", abs(x), (x < 0) ? 'D' : 'C');
    }
}

import utils::collections;

fn int dispatch(Scancode key_scancode, int fd)
{
    Scancode[] keys = {Scancode.KEY_UP, Scancode.KEY_DOWN, Scancode.KEY_RIGHT, Scancode.KEY_LEFT};
	static Vec2[] ops = {
	    [Scancode.KEY_UP]    = { 0, -1},
	    [Scancode.KEY_DOWN]  = { 0, +1},
	    [Scancode.KEY_RIGHT] = {+1,  0},
	    [Scancode.KEY_LEFT]  = {-1,  0},
	};

    // utils::string::printf(" %d, (%x) [%s] ", key_scancode, keyboard::codepage347::TABLE[key_scancode], Scancode.names[(int)key_scancode]);
    // utils::string::printf(" [%s] ", Scancode.names[(int)key_scancode]);

    if (try i = collections::one_of(<Scancode>)(keys, key_scancode)) {
		ecma48::handler::move_cursor(fd, ops[keys[i]]);
    }
    else {
        char c = keyboard::codepage347::TABLE[key_scancode];
        if (string::dprintf(fd, "%c", c) < 0) return -1;
	}
	return 0;
}

import keyboard;
import std_io;

fn int dispatch_on_key_pressed(int fd)
{
    Key_State key;
    if (keyboard::io::get_key_state(&key) && key.state == PRESSED) return dispatch(key.code, std_io::STD_IN);
    return 0;
}

