#ifndef KEYCODE_H
#define KEYCODE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BACKSPACE 8
#define PRESSED 0
#define RELEASED 1

typedef enum { SCAN_DEFAULT, SCAN_EXTENDED, SCAN_PAUSE } scanstatus_t;
typedef enum { LAYOUT_DEFAULT, LAYOUT_QWERTY, LAYOUT_AZERTY } keyboard_layout_t;

typedef enum {
	NONE,
	ESCAPE,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	MINUS,
	EQUAL,
	BCKSPC,
	TAB,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	LBRC,
	RBRC,
	ENTER,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	SMCLN,
	QUOT,
	GRV,
	BSLS,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	COMM,
	DOT,
	SLSH,
	ASTR,
	SPACE,
	CAPSLOCK,
	NUMLOCK,
	SCROLLLOCK,
	L_CONTROL,
	L_ALT,
	L_SHIFT,
	R_SHIFT,
	PREVIOUS_TRACK,
	NEXT_TRACK,
	R_CONTROL,
	MUTE,
	CALCULATOR,
	PLAY,
	STOP,
	VOL_DOWN,
	VOL_UP,
	WWW_HOME,
	R_ALT,
	HOME,
	UP,
	PAGE_UP,
	LEFT,
	RIGHT,
	END,
	DOWN,
	PAGE_DOWN,
	INSERT,
	DELETE,
	L_GUI,
	R_GUI,
	APPS,
	POWER,
	SLEEP,
	WAKE,
	WWW_SEARCH,
	WWW_FAVORITE,
	WWW_REFRESH,
	WWW_STOP,
	WWW_FORWARD,
	WWW_BACK,
	MY_COMPUTER,
	EMAIL,
	MEDIA_SELECT,
	PAUSE,
	FN_1,
	FN_2,
	FN_3,
	FN_4,
	FN_5,
	FN_6,
	FN_7,
	FN_8,
	FN_9,
	FN_10,
	FN_11,
	FN_12,
	PAD_1,
	PAD_2,
	PAD_3,
	PAD_4,
	PAD_5,
	PAD_6,
	PAD_7,
	PAD_8,
	PAD_9,
	PAD_0,
	PAD_SLASH,
	PAD_DOT,
	PAD_ASTR,
	PAD_MINUS,
	PAD_PLUS,
	PAD_ENTER,
} keycode_t;

#define NONNUMPAD                                                                          \
	END, DOWN, PAGE_DOWN, LEFT, NONE, RIGHT, HOME, UP, PAGE_UP, INSERT, PAD_SLASH, DELETE, \
		PAD_ASTR, PAD_MINUS, PAD_PLUS, PAD_ENTER

#define SCANCODES                                                                                  \
	[0x01] = ESCAPE, [0x02] = KEY_1, [0x03] = KEY_2, [0x04] = KEY_3, [0x05] = KEY_4,               \
	[0x06] = KEY_5, [0x07] = KEY_6, [0x08] = KEY_7, [0x09] = KEY_8, [0x0A] = KEY_9,                \
	[0x0B] = KEY_0, [0x0C] = MINUS, [0x0D] = EQUAL, [0x0E] = BCKSPC, [0x0F] = TAB, [0x10] = KEY_Q, \
	[0x11] = KEY_W, [0x12] = KEY_E, [0x13] = KEY_R, [0x14] = KEY_T, [0x15] = KEY_Y,                \
	[0x16] = KEY_U, [0x17] = KEY_I, [0x18] = KEY_O, [0x19] = KEY_P, [0x1A] = LBRC, [0x1B] = RBRC,  \
	[0x1C] = ENTER, [0x1D] = L_CONTROL, [0x1E] = KEY_A, [0x1F] = KEY_S, [0x20] = KEY_D,            \
	[0x21] = KEY_F, [0x22] = KEY_G, [0x23] = KEY_H, [0x24] = KEY_J, [0x25] = KEY_K,                \
	[0x26] = KEY_L, [0x27] = SMCLN, [0x28] = QUOT, [0x29] = GRV, [0x2A] = L_SHIFT, [0x2B] = BSLS,  \
	[0x2C] = KEY_Z, [0x2D] = KEY_X, [0x2E] = KEY_C, [0x2F] = KEY_V, [0x30] = KEY_B,                \
	[0x31] = KEY_N, [0x32] = KEY_M, [0x33] = COMM, [0x34] = DOT, [0x35] = SLSH, [0x36] = R_SHIFT,  \
	[0x37] = ASTR, [0x38] = L_ALT, [0x39] = SPACE, [0x3A] = CAPSLOCK, [0x3B] = FN_1,               \
	[0x3C] = FN_2, [0x3D] = FN_3, [0x3E] = FN_4, [0x3F] = FN_5, [0x40] = FN_6, [0x41] = FN_7,      \
	[0x42] = FN_8, [0x43] = FN_9, [0x44] = FN_10, [0x45] = NUMLOCK, [0x46] = SCROLLLOCK,           \
	[0x47] = PAD_7, [0x48] = PAD_8, [0x49] = PAD_9, [0x4A] = PAD_MINUS, [0x4B] = PAD_4,            \
	[0x4C] = PAD_5, [0x4D] = PAD_6, [0x4E] = PAD_PLUS, [0x4F] = PAD_1, [0x50] = PAD_2,             \
	[0x51] = PAD_3, [0x52] = PAD_0, [0x53] = PAD_DOT

#define EXTENDED_SCANCODES                                                                     \
	[0x10] = PREVIOUS_TRACK, [0x19] = NEXT_TRACK, [0x1C] = PAD_ENTER, [0x1D] = R_CONTROL,      \
	[0x20] = MUTE, [0x21] = CALCULATOR, [0x22] = PLAY, [0x24] = STOP, [0x2E] = VOL_DOWN,       \
	[0x30] = VOL_UP, [0x32] = WWW_HOME, [0x35] = PAD_SLASH, [0x38] = R_ALT, [0x47] = HOME,     \
	[0x48] = UP, [0x49] = PAGE_UP, [0x4B] = LEFT, [0x4D] = RIGHT, [0x4F] = END, [0x50] = DOWN, \
	[0x51] = PAGE_DOWN, [0x52] = INSERT, [0x53] = DELETE, [0x5B] = L_GUI, [0x5C] = R_GUI,      \
	[0x5D] = APPS, [0x5E] = POWER, [0x5F] = SLEEP, [0x63] = WAKE, [0x65] = WWW_SEARCH,         \
	[0x66] = WWW_FAVORITE, [0x67] = WWW_REFRESH, [0x68] = WWW_STOP, [0x69] = WWW_FORWARD,      \
	[0x6A] = WWW_BACK, [0x6B] = MY_COMPUTER, [0x6C] = EMAIL, [0x6D] = MEDIA_SELECT

#define KEYCODES_TO_QWERTY                                                                         \
	[ESCAPE] = {27, 27}, [KEY_1] = {'1', '!'}, [KEY_2] = {'2', '@'}, [KEY_3] = {'3', '#'},         \
	[KEY_4] = {'4', '$'}, [KEY_5] = {'5', '%'}, [KEY_6] = {'6', '^'}, [KEY_7] = {'7', '&'},        \
	[KEY_8] = {'8', '*'}, [KEY_9] = {'9', '('}, [KEY_0] = {'0', ')'}, [MINUS] = {'-', '_'},        \
	[EQUAL] = {'=', '+'}, [BCKSPC] = {8, 8}, [TAB] = {'\t', '\t'}, [KEY_Q] = {'q', 'Q'},           \
	[KEY_W] = {'w', 'W'}, [KEY_E] = {'e', 'E'}, [KEY_R] = {'r', 'R'}, [KEY_T] = {'t', 'T'},        \
	[KEY_Y] = {'y', 'Y'}, [KEY_U] = {'u', 'U'}, [KEY_I] = {'i', 'I'}, [KEY_O] = {'o', 'O'},        \
	[KEY_P] = {'p', 'P'}, [LBRC] = {'[', '{'}, [RBRC] = {']', '}'}, [ENTER] = {'\n', '\n'},        \
	[KEY_A] = {'a', 'A'}, [KEY_S] = {'s', 'S'}, [KEY_D] = {'d', 'D'}, [KEY_F] = {'f', 'F'},        \
	[KEY_G] = {'g', 'G'}, [KEY_H] = {'h', 'H'}, [KEY_J] = {'j', 'J'}, [KEY_K] = {'k', 'K'},        \
	[KEY_L] = {'l', 'L'}, [SMCLN] = {';', ':'}, [QUOT] = {'\'', '\"'}, [GRV] = {'`', '~'},         \
	[BSLS] = {'\\', '|'}, [KEY_Z] = {'z', 'Z'}, [KEY_X] = {'x', 'X'}, [KEY_C] = {'c', 'C'},        \
	[KEY_V] = {'v', 'V'}, [KEY_B] = {'b', 'B'}, [KEY_N] = {'n', 'N'}, [KEY_M] = {'m', 'M'},        \
	[COMM] = {',', '<'}, [DOT] = {'.', '>'}, [SLSH] = {'/', '?'}, [ASTR] = {'*', '*'},             \
	[SPACE] = {' ', ' '}, [CAPSLOCK] = {0, 0}, [NUMLOCK] = {0, 0}, [SCROLLLOCK] = {0, 0},          \
	[L_CONTROL] = {0, 0}, [R_CONTROL] = {0, 0}, [L_SHIFT] = {0, 0}, [R_SHIFT] = {0, 0},            \
	[L_ALT] = {0, 0}, [R_ALT] = {0, 0}, [PREVIOUS_TRACK] = {0, 0}, [NEXT_TRACK] = {0, 0},          \
	[MUTE] = {0, 0}, [CALCULATOR] = {0, 0}, [PLAY] = {0, 0}, [STOP] = {0, 0}, [VOL_DOWN] = {0, 0}, \
	[VOL_UP] = {0, 0}, [WWW_HOME] = {0, 0}, [HOME] = {0, 0}, [UP] = {0, 0}, [PAGE_UP] = {0, 0},    \
	[LEFT] = {0, 0}, [RIGHT] = {0, 0}, [END] = {0, 0}, [DOWN] = {0, 0}, [PAGE_DOWN] = {0, 0},      \
	[INSERT] = {0, 0}, [DELETE] = {0, 0}, [L_GUI] = {0, 0}, [R_GUI] = {0, 0}, [APPS] = {0, 0},     \
	[POWER] = {0, 0}, [SLEEP] = {0, 0}, [WAKE] = {0, 0}, [WWW_SEARCH] = {0, 0},                    \
	[WWW_FAVORITE] = {0, 0}, [WWW_REFRESH] = {0, 0}, [WWW_STOP] = {0, 0}, [WWW_FORWARD] = {0, 0},  \
	[WWW_BACK] = {0, 0}, [MY_COMPUTER] = {0, 0}, [EMAIL] = {0, 0}, [MEDIA_SELECT] = {0, 0},        \
	[PAUSE] = {0, 0}, [FN_1] = {0, 0}, [FN_2] = {0, 0}, [FN_3] = {0, 0}, [FN_4] = {0, 0},          \
	[FN_5] = {0, 0}, [FN_6] = {0, 0}, [FN_7] = {0, 0}, [FN_8] = {0, 0}, [FN_9] = {0, 0},           \
	[FN_10] = {0, 0}, [FN_11] = {0, 0}, [FN_12] = {0, 0}, [PAD_7] = {'7', '7'},                    \
	[PAD_8] = {'8', '8'}, [PAD_9] = {'9', '9'}, [PAD_MINUS] = {'-', '-'}, [PAD_4] = {'4', '4'},    \
	[PAD_5] = {'5', '5'}, [PAD_6] = {'6', '6'}, [PAD_PLUS] = {'+', '+'}, [PAD_1] = {'1', '1'},     \
	[PAD_2] = {'2', '2'}, [PAD_3] = {'3', '3'}, [PAD_0] = {'0', '0'}, [PAD_DOT] = {'.', '.'},      \
	[PAD_ENTER] = {'\n', '\n'}

#define KEYCODES_TO_AZERTY                                                                         \
	[ESCAPE] = {27, 27}, [KEY_1] = {'&', '1'}, [KEY_2] = {130, '2'}, [KEY_3] = {'"', '3'},         \
	[KEY_4] = {'\'', '4'}, [KEY_5] = {'(', '5'}, [KEY_6] = {'-', '6'}, [KEY_7] = {138, '7'},       \
	[KEY_8] = {'_', '8'}, [KEY_9] = {135, '9'}, [KEY_0] = {133, '0'}, [MINUS] = {')', 248},        \
	[EQUAL] = {'=', '+'}, [BCKSPC] = {8, 8}, [TAB] = {'\t', '\t'}, [KEY_Q] = {'a', 'A'},           \
	[KEY_W] = {'z', 'Z'}, [KEY_E] = {'e', 'E'}, [KEY_R] = {'r', 'R'}, [KEY_T] = {'t', 'T'},        \
	[KEY_Y] = {'y', 'Y'}, [KEY_U] = {'u', 'U'}, [KEY_I] = {'i', 'I'}, [KEY_O] = {'o', 'O'},        \
	[KEY_P] = {'p', 'P'}, [LBRC] = {'^', 249}, [RBRC] = {'$', 156}, [ENTER] = {'\n', '\n'},        \
	[KEY_A] = {'q', 'Q'}, [KEY_S] = {'s', 'S'}, [KEY_D] = {'d', 'D'}, [KEY_F] = {'f', 'F'},        \
	[KEY_G] = {'g', 'G'}, [KEY_H] = {'h', 'H'}, [KEY_J] = {'j', 'J'}, [KEY_K] = {'k', 'K'},        \
	[KEY_L] = {'l', 'L'}, [SMCLN] = {'m', 'M'}, [QUOT] = {151, '%'}, [GRV] = {253, 253},           \
	[BSLS] = {'*', 230}, [KEY_Z] = {'w', 'W'}, [KEY_X] = {'x', 'X'}, [KEY_C] = {'c', 'C'},         \
	[KEY_V] = {'v', 'V'}, [KEY_B] = {'b', 'B'}, [KEY_N] = {'n', 'N'}, [KEY_M] = {',', '?'},        \
	[COMM] = {';', '.'}, [DOT] = {':', '/'}, [SLSH] = {'!', 21}, [ASTR] = {'*', '*'},              \
	[SPACE] = {' ', ' '}, [CAPSLOCK] = {0, 0}, [NUMLOCK] = {0, 0}, [SCROLLLOCK] = {0, 0},          \
	[L_CONTROL] = {0, 0}, [R_CONTROL] = {0, 0}, [L_SHIFT] = {0, 0}, [R_SHIFT] = {0, 0},            \
	[L_ALT] = {0, 0}, [R_ALT] = {0, 0}, [PREVIOUS_TRACK] = {0, 0}, [NEXT_TRACK] = {0, 0},          \
	[MUTE] = {0, 0}, [CALCULATOR] = {0, 0}, [PLAY] = {0, 0}, [STOP] = {0, 0}, [VOL_DOWN] = {0, 0}, \
	[VOL_UP] = {0, 0}, [WWW_HOME] = {0, 0}, [HOME] = {0, 0}, [UP] = {0, 0}, [PAGE_UP] = {0, 0},    \
	[LEFT] = {0, 0}, [RIGHT] = {0, 0}, [END] = {0, 0}, [DOWN] = {0, 0}, [PAGE_DOWN] = {0, 0},      \
	[INSERT] = {0, 0}, [DELETE] = {0, 0}, [L_GUI] = {0, 0}, [R_GUI] = {0, 0}, [APPS] = {0, 0},     \
	[POWER] = {0, 0}, [SLEEP] = {0, 0}, [WAKE] = {0, 0}, [WWW_SEARCH] = {0, 0},                    \
	[WWW_FAVORITE] = {0, 0}, [WWW_REFRESH] = {0, 0}, [WWW_STOP] = {0, 0}, [WWW_FORWARD] = {0, 0},  \
	[WWW_BACK] = {0, 0}, [MY_COMPUTER] = {0, 0}, [EMAIL] = {0, 0}, [MEDIA_SELECT] = {0, 0},        \
	[PAUSE] = {0, 0}, [FN_1] = {0, 0}, [FN_2] = {0, 0}, [FN_3] = {0, 0}, [FN_4] = {0, 0},          \
	[FN_5] = {0, 0}, [FN_6] = {0, 0}, [FN_7] = {0, 0}, [FN_8] = {0, 0}, [FN_9] = {0, 0},           \
	[FN_10] = {0, 0}, [FN_11] = {0, 0}, [FN_12] = {0, 0}, [PAD_7] = {'7', '7'},                    \
	[PAD_8] = {'8', '8'}, [PAD_9] = {'9', '9'}, [PAD_MINUS] = {'-', '-'}, [PAD_4] = {'4', '4'},    \
	[PAD_5] = {'5', '5'}, [PAD_6] = {'6', '6'}, [PAD_PLUS] = {'+', '+'}, [PAD_1] = {'1', '1'},     \
	[PAD_2] = {'2', '2'}, [PAD_3] = {'3', '3'}, [PAD_0] = {'0', '0'}, [PAD_DOT] = {'.', '.'},      \
	[PAD_ENTER] = {'\n', '\n'}

/* ==================================== scancode ================================================ */
uint8_t handle_scancode_default();
uint8_t handle_scancode_extended();
uint8_t handle_scancode_pause();
unsigned char const (*keyboard_layout(keyboard_layout_t layout))[2];

#endif
