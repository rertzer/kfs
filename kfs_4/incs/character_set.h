#ifndef CHARACTER_SET_H
#define CHARACTER_SET_H

typedef enum s_box_drawing_code {
    VERTICAL_LINE               = 0xB3, // ─  (Horizontal Line)
    HORIZONTAL_LINE             = 0xC4, // │  (Vertical Line)
    TOP_LEFT_CORNER             = 0xDA, // ┌  (Top Left Corner)
    TOP_RIGHT_CORNER            = 0xBF, // ┐  (Top Right Corner)
    BOTTOM_LEFT_CORNER          = 0xC0, // └  (Bottom Left Corner)
    BOTTOM_RIGHT_CORNER         = 0xD9, // ┘  (Bottom Right Corner)
    LEFT_TEE                    = 0xC3, // ├  (Left Tee)
    RIGHT_TEE                   = 0xB4, // ┤  (Right Tee)
    TOP_TEE                     = 0xC2, // ┬  (Top Tee)
    BOTTOM_TEE                  = 0xC1, // ┴  (Bottom Tee)
    CROSS                       = 0xC5, // ┼  (Cross)
    DOUBLE_HORIZONTAL_LINE      = 0xCD, // ═  (Double Horizontal Line)
    DOUBLE_VERTICAL_LINE        = 0xBA, // ║  (Double Vertical Line)
    DOUBLE_TOP_LEFT_CORNER      = 0xC9, // ╔  (Double Top Left Corner)
    DOUBLE_TOP_RIGHT_CORNER     = 0xBB, // ╗  (Double Top Right Corner)
    DOUBLE_BOTTOM_LEFT_CORNER   = 0xC8, // ╚  (Double Bottom Left Corner)
    DOUBLE_BOTTOM_RIGHT_CORNER  = 0xBC, // ╝  (Double Bottom Right Corner)
    DOUBLE_LEFT_TEE             = 0xCC, // ╠  (Double Left Tee)
    DOUBLE_RIGHT_TEE            = 0xB9, // ╣  (Double Right Tee)
    DOUBLE_TOP_TEE              = 0xCB, // ╦  (Double Top Tee)
    DOUBLE_BOTTOM_TEE           = 0xCA, // ╩  (Double Bottom Tee)
    DOUBLE_CROSS                = 0xCE, // ╬  (Double Cross)
} t_box_drawing_code;

/*
typedef struct {
    unsigned int        unicode;    // Unicode code point
    t_box_drawing_code  cp437;      // Corresponding CP437 value
} t_box_mapping;

t_box_mapping box_map[] = {
    {0x2500, HORIZONTAL_LINE},
    {0x2502, VERTICAL_LINE},
    {0x250C, TOP_LEFT_CORNER},
    {0x2510, TOP_RIGHT_CORNER},
    {0x2514, BOTTOM_LEFT_CORNER},
    {0x2518, BOTTOM_RIGHT_CORNER},
    {0x251C, LEFT_TEE},
    {0x2524, RIGHT_TEE},
    {0x252C, TOP_TEE},
    {0x2534, BOTTOM_TEE},
    {0x253C, CROSS},
    {0x2550, DOUBLE_HORIZONTAL_LINE},
    {0x2551, DOUBLE_VERTICAL_LINE},
    {0x2554, DOUBLE_TOP_LEFT_CORNER},
    {0x2557, DOUBLE_TOP_RIGHT_CORNER},
    {0x255A, DOUBLE_BOTTOM_LEFT_CORNER},
    {0x255D, DOUBLE_BOTTOM_RIGHT_CORNER},
    {0x2560, DOUBLE_LEFT_TEE},
    {0x2563, DOUBLE_RIGHT_TEE},
    {0x2566, DOUBLE_TOP_TEE},
    {0x2569, DOUBLE_BOTTOM_TEE},
    {0x256C, DOUBLE_CROSS},
};

unsigned char unicode_to_cp437(unsigned int unicode)
{
    for (size_t i = 0; i < sizeof(box_map) / sizeof(t_box_mapping); i++) {
        if (box_map[i].unicode == unicode) {
            return box_map[i].cp437;
        }
    }
    return '?';  // Default for unknown characters
}
*/

#endif // CHARACTER_SET_H