#include "kernel.h"
#include "builtin.h"
#include "gdt.h"
#include "keycode.h"
#include "memory.h"
#include "panic.h"
#include "terminal.h"

#include "malloc.h"

extern volatile uint8_t current_code;

static void process_keyboard(keypress_t* keypress);

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_pit();
	printk("- Programmable Interval Timer OK\n");
	init_idt();
	keypress_t keypress = init_keypress();
	invalidate_low_kernel();
	printk("- Low Kernel addresses invalidation OK\n");
	init_memory();
	init_v_memory();
	printk("- memory OK\n");
	printk("jrOS ready. Enjoy!\n");

	// memory_map_infos();
	// page_testing();
	// panic("Kernel test");
	// mbook_test();
	//press_any();
	// memory_test_k_mmap();
	//memory_test_vmbook();
	// term_prompt();
	// term_putstr("echo 42\n");
	// readline();
	memory_infos(NULL, 0);
	press_any();
	uint8_t* toto = vmalloc(256);
	memory_infos(NULL, 0);
	
	if (toto == NULL){
		printk("c'est nul!\n");
	}
	else {
		toto[0] = 'y';
		toto[1] = 'o';
		toto[2] = 'u';
		toto[3] = 'p';
		toto[4] = 'y';
		toto[5] = '\0';
			printk("toto = %s\n", toto);

		vfree(toto);
	}
	press_any();
	memory_infos(NULL, 0);

	term_prompt();
	while (true) {
		sleep();
		process_keyboard(&keypress);
	}
}

static void process_keyboard(keypress_t* keypress) {
	bool getline = false;
	keypress->keycode = current_code;
	current_code = 0;
	if (keypress->keycode != 0) {
		update_keypress(keypress);
		if (keypress->pressed == PRESSED) {
			getline = handle_keypress(*keypress);
		}
	}
	if (getline == true) {
		readline();
		term_prompt();
	}
}
