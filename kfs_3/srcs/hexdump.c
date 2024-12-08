#include "kernel.h"

extern uint32_t			stack_top;
extern uint32_t			stack_bottom;
extern volatile uint8_t current_code;
extern uint32_t			page_dir;
extern uint32_t			low_kernel_page_table;

static bool sleepy();

void print_line(uint32_t* stack_pointer) {
	uint32_t* stack_pointer_end = stack_pointer + 6;
	printk("%08x	", stack_pointer);
	for (uint32_t* i = stack_pointer; i < stack_pointer_end; i++) {
		printk("%08x ", *i);
	}
	printk("\n");
}
static bool sleepy() {
	keypress_t keypress = init_keypress();
	bool	   end = false;

	while (true) {
		keypress.keycode = current_code;
		current_code = 0;
		if (keypress.keycode != 0) {
			keypress = update_keypress(keypress);

			if (keypress.pressed == PRESSED) {
				if (keypress.ascii == 'q' || keypress.ascii == 'Q') {
					end = true;
					break;
				} else if ((keypress.ascii == 'n') || (keypress.ascii == 'N')) {
					break;
				}
			}
		}
		sleep();
	}
	return (end);
}

void tabledump(void) {
	printk("page dir at   0x%08x\n", &page_dir);
	for (uint32_t* i = &page_dir; i < &page_dir + 4; ++i) {
		printk("%08x \n", *i);
	}
	uint32_t* hp = &page_dir;
	hp += 768;
	uint32_t* high_kernel_page_table = &low_kernel_page_table;
	high_kernel_page_table += 0xC0000000 / 4;
	printk("dir 768: 0x%08x\n", *hp);
	printk("high page at  0x%08x\n", high_kernel_page_table);
	uint32_t index = 0;

	for (uint32_t* i = high_kernel_page_table; i < high_kernel_page_table + 4; ++i) {
		printk("%08x: %08x \n", index, *i);
		++index;
	}
	// printk("setgdt: 0x%08x\n", set_gdt);
	// printk("init_gdt: 0x%08x\n", init_gdt);
}

uint8_t hexdump(void) {
	current_code = 0;
	// volatile char testB[6000];
	// for (size_t i = 0; i < 6000; i++) {
	// 	testB[i] = 'B';
	// }
	uint32_t counter = 0;

	for (uint32_t* stack_pointer = &stack_top; stack_pointer <= &stack_bottom; stack_pointer++) {
		print_line(stack_pointer);
		++counter;
		if (counter == 15) {
			counter = 0;
			if (sleepy()) {
				break;
			}
		}
	}

	printk("\nStack bottom: 0x%08x\n", &stack_top);
	printk("Stack top:    0x%08x\n", &stack_bottom);
	printk("Stack size: %d\n", (uint32_t)&stack_bottom - (uint32_t)&stack_top);

	return (0);
}
