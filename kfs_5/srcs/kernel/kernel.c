#include "kernel.h"
#include "builtin.h"
#include "gdt.h"
#include "interrupts.h"
#include "keycode.h"
#include "memory.h"
#include "panic.h"
#include "terminal.h"
#include "tss.h"
#include "utils.h"

extern volatile uint8_t current_code;

static void process_keyboard(keypress_t* keypress);

void kernel_main(void) {
	all_terms_init();
	init_gdt();
	init_PIC();
	init_pit();
	init_idt();
	invalidate_low_kernel();
	init_memory();
	init_v_memory();
	// run_task_zero();
	kernel_zero();
}

void kernel_zero() {
	// readdump(0, NULL);
	// press_any();
	// boot_infos(0, NULL);
	// page_testing();
	// panic("Kernel test");
	// mbook_test();
	// memory_test_vmbook();
	// test_malloc(NULL, set_simple_test, set_complex_test, NULL);
	// interrupts_test();

	// dump_stack();
	// press_any();
	// reboot(0, NULL);
	// arg_split_test();

	printk("jrOS ready. Welcome to kernel zero. Enjoy!\n");
	int_allowed();
	keypress_t keypress = init_keypress();
	term_prompt();
	uint16_t tr = store_task_register();
	printk("TR: %08x\n", tr);
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
