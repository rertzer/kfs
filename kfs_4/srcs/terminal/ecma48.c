#include "ecma48.h"

#include "unistd.h"

#define STD_IO_BUFFER_SIZE  128
#define ESCAPE_SEQUENCE             '\e'
#define CONTROL_SEQUENCE_INTRODUCER 0x5B

int parse_input_parameter(char* input, int* output[16]) {
    int index = 0;
    int output_index = 0;

    while (input[index] && output_index < 16) {
        if(input[index] == ';') {
            index += 1;
            output_index += 1;
            continue;
        }
        if (is_digit(input[index])) {
            index += atoi(&output[output_index], &input[index]);
            continue;
        }
        return index;
    }
    return index;
}

static int dispatch(t_ecma48_handlers *this, void* ref, char* input) {
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
                    this->on_cursor_mouvement(ref, 0, -params[0]);
                    break;
                case 'B':
                    this->on_cursor_mouvement(ref, 0, +params[0]);
                    break;
                case 'C':
                    this->on_cursor_mouvement(ref, +params[0], 0);
                    break;
                case 'D':
                    this->on_cursor_mouvement(ref, -params[0], 0);
                    break;
                case 'H':
                    if (this->on_set_cursor_position) {
                        this->on_set_cursor_position(ref, params[1], params[0]);
                    }
                    break;
                case 'f':
                    if (this->on_cursor_mouvement) {
                        this->on_cursor_mouvement(ref, params[1], params[0]);
                    }
                    break;
                case 'S':
                    if (this->on_scroll_up) {
                        this->on_scroll_up(ref, params[0]);
                    } break;
                case 'T':
                    if (this->on_scroll_down) {
                        this->on_scroll_down(ref, params[0]);
                    } break;
                // case 'm':
                //     if (this->on_graphic_rendition) {
                //         this->on_graphic_rendition(ref, params[0]);
                //     } break;
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

void hooks(t_ecma48_handlers* this, void* ref, char* buffer, size_t size) {
    int index = 0;
    while (index < size) {
        index += dispatch(this, ref, &buffer[index]);
    }
}

void hooks_fd(t_ecma48_handlers* this, void* ref, int fd) {
	int     read_size = 0;
    char    read_buffer[STD_IO_BUFFER_SIZE] = {};
    
    read_size = read(fd, &read_buffer, STD_IO_BUFFER_SIZE);
	if (read_size > 0) {
        hooks(this, ref, &read_buffer, read_size);
    }
}
