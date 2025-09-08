#include "kernel.h"
#include "builtin.h"
#include "gdt.h"
#include "interrupts.h"
#include "keycode.h"
#include "memory.h"
#include "panic.h"
#include "terminal.h"
#include "utils.h"

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
	// readdump(0, NULL);
	// press_any();
	// boot_infos(0, NULL);
	// page_testing();
	// panic("Kernel test");
	// mbook_test();
	// memory_test_vmbook();
	// test_malloc(NULL, set_simple_test, set_complex_test, NULL);
	// interrupts_test();

	// STACK DUMP TEST
	// dump_stack();
	// press_any();
	// reboot(0, NULL);
	// arg_split_test();

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
		process_line();
		term_prompt();
	}
}
