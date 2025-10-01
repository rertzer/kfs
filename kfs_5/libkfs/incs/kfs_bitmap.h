#ifndef KFS_BITMAP_H
#define KFS_BITMAP_H

#include <stdbool.h>
#include <stddef.h>

void   bitmap_erase(size_t* bitmap, size_t size);
bool   get_bitmap_value(size_t* bitmap, size_t offset);
void   set_bitmap_value(size_t* bitmap, size_t offset, bool value);
size_t get_next_bitmap(size_t* bitmap, size_t size, size_t offset);

#endif
