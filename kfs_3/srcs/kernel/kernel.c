#include "kernel.h"
#include "boot_infos.h"
#include "builtin.h"
#include "gdt.h"
#include "keycode.h"
#include "memory.h"
#include "panic.h"
#include "terminal.h"

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
	init_memory();
	init_v_memory();
	// virtual_memory_infos(NULL, 0);
	// memory_map_infos();

	// uint32_t mem_size = get_mem_size();
	// printk("memory size: %u\n", mem_size);
	//
	// for (int i = 0; i < 12; ++i) {
	// 	printk("---- %u ----\n", i);
	// 	// memory_infos(NULL, 0);
	// 	void*	 addr = v_mmap((1 << i) * 4096, SUPERVISOR_LEVEL, READ_WRITE);
	// 	uint32_t size = v_size(addr);
	// 	printk("%u, address %08x size %u\n\n", i, addr, size);
	// 	// memory_infos(NULL, 0);
	// 	virtual_memory_infos(NULL, 0);
	// 	v_free(addr);
	// 	press_any();
	// }
	// memory_map_infos();
	// page_testing();
	press_any();
	// panic("Kernel test\n");
	// mbook_test();
	// godot();
	term_putstr("echo 42\n");
	// term_putstr("memoryinfos\n");
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
