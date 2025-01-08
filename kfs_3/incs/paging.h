#ifndef PAGING_H
#define PAGING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint32_t* get_dir_page();
uint32_t  get_dir_page_offset(uint32_t l_address);
uint32_t  create_page_table(uint32_t offset);
#endif
