#ifndef PANIC_H
#define PANIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PANIC_HEADER_OFFSET 25
#define PANIC_HEADER_WIDTH 30
#define PANIC_HEADER_HIGH 7
#define PANIC_ROW_START PANIC_HEADER_HIGH + 1

/*Fire font ASCII art*/
#define PANIC_HEADER                                                                                 \
	"         )        (       ", "`  )   ( /(   (    )\\   (   ", "/(/(   )(_))  )\\ )((_)  )\\  ", \
		"((_)_\\ ((_)_  _(_/( (_) ((_) ", "| '_ \\)/ _` || ' \\))| |/ _|  ", "| .__/ \\__,_||_||_| |_|\\__|  ", "|_|"

void panic(char* msg);
void dump_stack();
void read_dump();

#endif
