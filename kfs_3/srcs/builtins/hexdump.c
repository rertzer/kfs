#include "kernel.h"
#include "keycode.h"
#include "paging.h"

extern uint32_t			stack_top;
extern uint32_t			stack_bottom;
extern volatile uint8_t current_code;
extern uint32_t			page_dir;
extern uint32_t			low_kernel_page_table;

static void print_line(uint32_t* line);
static bool dump_stop(uint8_t* counter);
static bool sleeping();
static bool read_keyboard(keypress_t* kp);

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

uint8_t hexdump(char* pointer, size_t len) {
	(void)pointer;
	(void)len;
	current_code = 0;
	uint8_t counter = 0;

	for (uint32_t* line = &stack_top; line <= &stack_bottom; ++line) {
		print_line(line);
		if (dump_stop(&counter)) {
			break;
		}
	}

	printk("\nStack bottom: 0x%08x\n", &stack_top);
	printk("Stack top:    0x%08x\n", &stack_bottom);
	printk("Stack size: %d\n", (uint32_t)&stack_bottom - (uint32_t)&stack_top);

	return (0);
}

static void print_line(uint32_t* line) {
	printk("%08x	", line);
	for (uint32_t i = 0; i < 6; ++i) {
		printk("%08x ", line[i]);
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
		*kp = update_keypress(*kp);
		if (kp->pressed == PRESSED) {
			update = true;
		}
	}
	return (update);
}
