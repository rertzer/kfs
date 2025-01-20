#include "kernel.h"
#include "boot_infos.h"
#include "memory.h"
// #include "builtin.h"

extern volatile uint8_t current_code;

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_pit();
	init_idt();
	keypress_t keypress = init_keypress();
	invalidate_low_kernel();
	// printk("boom at 0x%08x\n", boom);
	// boom();
	// page_testing();
	init_memory();
	// memory_map_infos();

	// uint32_t mem_size = get_mem_size();
	// printk("memory size: %u\n", mem_size);

	// for (int i = 0; i < 15; ++i) {
	// 	void* addr = k_mmap((1 << 4) * 4096);
	// 	printk("%u, %08x\n", i, addr);
	// }

	// godot();
	term_putstr("echo 42\n");
	readline();
	term_prompt();
	while (true) {
		sleep();
		bool getline = false;
		keypress.keycode = current_code;
		current_code = 0;
		if (keypress.keycode != 0) {
			keypress = update_keypress(keypress);
			if (keypress.pressed == PRESSED) {
				getline = handle_keypress(keypress);
			}
		}
		if (getline == true) {
			readline();
			term_prompt();
		}
	}
}
