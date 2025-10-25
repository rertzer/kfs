#include "processus.h"
#include "gdt.h"
#include "malloc.h"
#include "string.h"

proc_t* init_zero_proc() {
	proc_t* proc = kmalloc(sizeof(proc_t));
	if (proc == NULL) {
		return (NULL);
	}
	proc->pid = 0;
	proc->owner = 0;
	proc->parent = proc;
	proc->gdt_index = 0;
	proc->tss = get_tss_addr_by_gdt_offset(TSS_ZERO * sizeof(gdt_entry_t));
	// proc->signals = NULL;
	proc->sig_pending = 0;
	proc->sig_processing = 0;
	proc->status = PROC_SLEEP;
	proc->kernel_stack = NULL;
	list_head_init(&proc->lst);
	list_head_init(&proc->run_lst);
	list_head_init(&proc->childrens);
	list_head_init(&proc->siblings);
	list_head_init(&proc->heap);

	return (proc);
}

proc_t* spawn(proc_t* parent, caller_data_t fc) {
	proc_t* child = kmalloc(sizeof(proc_t));
	if (child == NULL) {
		return (NULL);
	}
	child->pid = pid_bitmap_get_new();
	if (child->pid == 0) {
		kfree(child);
		return (NULL);
	}
	child->kernel_stack = mbook(KERNEL_STACK_SIZE, SUPERVISOR_LEVEL, READ_WRITE);

	if (child->kernel_stack == NULL) {
		pid_bitmap_remove(child->pid);
		kfree(child);
		return (NULL);
	}
	copy_stack((uint8_t*)fc.old_esp, child->kernel_stack);
	fc = switch_fc_baseline(fc, child->kernel_stack);

	child->owner = parent->owner;
	child->parent = parent;
	child->gdt_index = 0;
	child->tss = spawn_tss(fc);
	if (child->tss == NULL) {
		pid_bitmap_remove(child->pid);
		kfree(child);
		kfree(child->kernel_stack);
		return (NULL);
	}
	// child->signals = NULL;
	child->sig_pending = 0;
	child->sig_processing = 0;
	child->status = PROC_SLEEP;
	// create heap
	// parenthood handled by the scheduler
	list_head_init(&child->lst);
	list_head_init(&child->run_lst);
	list_head_init(&child->childrens);
	list_head_init(&child->siblings);
	list_head_init(&child->heap);

	return (child);
}

void copy_stack(uint8_t* old_esp, uint8_t* low_stack) {
	uint8_t* high_src = get_kernel_stack_high(old_esp);
	uint8_t* high_dest = get_kernel_stack_high(low_stack);
	uint32_t stack_len = (uint32_t)high_src - (uint32_t)old_esp + 1;
	printf("copy stack %p %p len %d\n", high_src, old_esp, stack_len);
	for (size_t i = 0; i < stack_len; ++i) {
		*(high_dest - i) = *(high_src - i);
	}
}

caller_data_t switch_fc_baseline(caller_data_t cd, uint8_t* baseline) {
	cd.old_esp &= KERNEL_STACK_HIGH_MASK;
	cd.old_ebp &= KERNEL_STACK_HIGH_MASK;
	cd.old_esp |= (uint32_t)baseline;
	cd.old_ebp |= (uint32_t)baseline;

	return (cd);
}

uint8_t* get_kernel_stack_high(uint8_t* sp) {
	uint32_t stack_high = (uint32_t)sp;
	stack_high |= KERNEL_STACK_HIGH_MASK;

	printf("stack high is %x\t", stack_high);
	return ((uint8_t*)stack_high);
}

void free_process(proc_t* task) {
	pid_bitmap_remove(task->pid);
	// remove from family
	kfree(task->tss);
	munbook(task->kernel_stack);
	// free memory
	kfree(task);
}

void print_process(void* vtask) {
	proc_t* task = (proc_t*)vtask;
	printf("pid: %d\tppid: %d\towner: %d\tstatus: %d\n", task->pid, task->parent->pid, task->owner, task->status);
}

void proc_set_gdt_index(proc_t* task, uint32_t gdt_index) {
	task->gdt_index = gdt_index;
}
