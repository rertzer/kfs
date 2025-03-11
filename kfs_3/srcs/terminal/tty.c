#include "tty.h"
#include "ecma48.h"
#include "vga.h"

// TODO: finish tty_remove_entry

static size_t   tty_get_frame_buffer_size(t_tty *tty) {
    return (tty->frame.height * tty->frame.width);
}

static size_t   tty_compute_linear_position(t_tty *tty, size_t x, size_t y) {
	return (y * tty->frame.width + x);
}

static size_t   tty_get_cursor_linear_position(t_tty *tty) {
    return (tty_compute_linear_position(tty, tty->cursor.x, tty->cursor.y));
}

static void     tty_insert_entries(t_tty *tty, uint16_t *entries, size_t n) {
    size_t  size = tty_get_frame_buffer_size(tty);
    size_t  caret = tty_get_cursor_linear_position(tty);
    memmove(
        &tty->frame.buffer[caret + n],
        &tty->frame.buffer[caret],
        (size - n) - caret);
    memcpy(&tty->frame.buffer[caret], entries, n * sizeof(uint16_t));
}

static void     tty_insert_char(t_tty *tty, char c) {
    uint16_t entry[1]   = {vga_char(c, tty->color)};
    tty_insert_entries(tty, entry, 1);
}

static void     tty_remove_entry(t_tty *tty, int n) {
    size_t  size = tty_get_frame_buffer_size(tty);
    size_t  caret = tty_get_cursor_linear_position(tty);
    memmove(
        &tty->frame.buffer[caret],
        &tty->frame.buffer[caret + n],
        (size - n) - caret);

	tty->frame.buffer[caret] = vga_char(' ', tty->color);
}

static void     tty_set_cursor_position(t_tty *tty, size_t x, size_t y) {
    tty->cursor.x = clamp(x, 0, tty->frame.width);
    tty->cursor.y = clamp(x, 0, tty->frame.height);
}

static void     tty_move_cursor_position_by(t_tty *tty, int32_t n) {
    size_t  caret = tty_get_cursor_linear_position(tty) + n;
    tty_set_cursor_position(
        tty,
        caret % tty->frame.width,
        caret / tty->frame.width);
}

static void     tty_fill_frame(t_tty *tty, uint16_t entry) {
    size_t  size = tty_get_frame_buffer_size(tty);
    for (size_t i = 0; i < size; i += 1) {
        tty->frame.buffer[i] = entry;
    }
}

static void     tty_goto_next_line(t_tty *tty) {
    tty_set_cursor_position(tty, 0, tty->cursor.y + 1);
}


/* hook handlers */
static void tty_on_cursor_mouvement(void *self, int32_t x, int32_t y) {
    tty_move_cursor_position_by(self, x);
}

static void tty_on_set_cursor_position(void *self, int32_t x, int32_t y) {
    tty_set_cursor_position(self, x, y);
}

static void tty_on_clear_screen(void *self) {
    t_tty   *this = self;

    tty_fill_frame(this, vga_char(' ', this->color));
    tty_set_cursor_position(this, 0, 0);
}

static void tty_on_scroll_up(void* self, int32_t n) {
    t_tty   *this = self;
}

static void tty_on_scroll_down(void* self, int32_t n) {
    t_tty   *this = self;
}

/*
static void tty_on_graphic_rendition(void *self, ...) {
	uint n = args[0];

	if (n == 0) {
		return; // reset all
	}
	if (30 <= n && n <= 37) { // Set foreground color
		this.current_color.foreground = Color.values[n - 30];
		return;
	}
	if (40 <= n && n <= 47) { // Set background color
		this.current_color.background = Color.values[n - 40];
		return;
	}
}
*/

static void tty_on_next_line(void* self) {
    tty_goto_next_line(self);
}

static void tty_on_default(void* self, char c) {
    tty_insert_char(self, c);
    tty_move_cursor_position_by(self, 1);
}

static void tty_on_backspace(void* self, char c) {
    (void)c;
    tty_remove_entry(self, -1);
    tty_move_cursor_position_by(self, -1);
}

static void tty_on_delete(void* self, char c) {
    (void)c;
    tty_remove_entry(self, 1);
}

static void tty_on_enter(void* self, char c) {
    (void)c;
    tty_goto_next_line(self);
}

static const t_ecma48_handlers ecma48_handlers = {
    .on_cursor_mouvement    = tty_on_cursor_mouvement,
    .on_set_cursor_position = tty_on_set_cursor_position,
    .on_clear_screen        = tty_on_clear_screen,
    .on_scroll_up			= tty_on_scroll_up,
	.on_scroll_down			= tty_on_scroll_down,
	// .on_graphic_rendition	= tty_on_graphic_rendition,
	.on_next_line			= tty_on_next_line,
    .default_char_handler   = tty_on_default,
    .char_handlers = {
        [0x08]              = tty_on_backspace,
        [0x7F]              = tty_on_delete,
        [0x0A]              = tty_on_enter,
    }
};


/*
fn void Tty.init(Tty *this)
{
    this.default_color = Entry_color{Color.LIGHT_GREY, Color.BLACK};
	this.current_color = this.default_color;

    this.vga_frame.init();
    this.vga_frame.fill(vga::entry(' ', this.default_color));
}

fn void	Tty.update(Tty* this)
{
	handlers::HANDLER.hooks_fd(this, std_io::STD_OUT);
	this.vga_frame.main_frame_update();
}

fn int Tty.handle_input(Tty this)
{
	return ecma48::handler::dispatch_on_key_pressed(std_io::STD_IN);
}
*/
