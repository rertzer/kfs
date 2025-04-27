#include "kernel.h"
#include "keycode.h"
#include "paging.h"

extern uint32_t			stack_top;
extern uint32_t			stack_bottom;
extern volatile uint8_t current_code;
extern uint32_t			page_dir;
extern uint32_t			low_kernel_page_table;
extern uint32_t*		dump_addr;

static void dump(uint32_t* start, uint32_t* end);
static void print_line(uint32_t* line);
static bool dump_stop(uint8_t* counter);
static bool sleeping();
static bool read_keyboard(keypress_t* kp);

uint8_t hexdump(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	dump(&stack_top, &stack_bottom);
	return (0);
}

uint8_t readdump(size_t argc, char** argv) {
	(void)argc;
	(void)argv;
	uint32_t stack_size = ((uint32_t)&stack_bottom - (uint32_t)&stack_top) / 4;
	dump(dump_addr, dump_addr + stack_size);
	return (0);
}

static void dump(uint32_t* start, uint32_t* end) {
	current_code = 0;
	uint8_t counter = 0;

	for (uint32_t* line = start; line <= end; ++line) {
		print_line(line);
		if (dump_stop(&counter)) {
			break;
		}
	}
	printk("\nStack bottom: %08x\n", end);
	printk("Stack top:    %08x\n", start);
	printk("Stack size dump: %d\n", (uint32_t)end - (uint32_t)start);
}

static void print_line(uint32_t* line) {
	printk("%08y	", line);
	for (uint32_t i = 0; i < 6; ++i) {
		printk("%08y ", line[i]);
	}
	printk("\n");
}

static bool dump_stop(uint8_t* counter) {
	bool quit = false;
	++*counter;

	if (*counter == 15) {
		*counter = 0;
		quit = sleeping();
	}
	return (quit);
}

static bool sleeping() {
	keypress_t keypress = init_keypress();
	bool	   end = false;
	while (true) {
		if (read_keyboard(&keypress) == true) {
			if (keypress.ascii == 'q' || keypress.ascii == 'Q') {
				end = true;
				break;
			} else if ((keypress.ascii == 'n') || (keypress.ascii == 'N')) {
				break;
			}
			sleep();
		}
	}
	return (end);
}

static bool read_keyboard(keypress_t* kp) {
	bool update = false;
	kp->keycode = current_code;
	current_code = 0;
	if (kp->keycode != 0) {
		update_keypress(kp);
		if (kp->pressed == PRESSED) {
			update = true;
		}
	}
	return (update);
}

void tabledump(void) {
	// printk("page dir at   0x%08x\n", &page_dir);
	// for (uint32_t* i = &page_dir; i < &page_dir + 4; ++i) {
	// 	printk("%08x \n", *i);
	// }
	printk("tabledump\n");

	uint32_t* dir_entry = (uint32_t*)PAGE_DIR_ADDR;
	for (uint32_t i = 830; i < 840; ++i) {
		printk("%u: %08x \n", i, *(dir_entry + i));
	}
	// printk("setgdt: 0x%08x\n", set_gdt);
	// printk("init_gdt: 0x%08x\n", init_gdt);
}
