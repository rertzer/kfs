#ifndef TSS_H
#define TSS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gdt.h"

#define TSS_FLAGS 0

#define TSS_BUSY_OFFSET 1
#define TSS_AVAIL (0 << TSS_BUSY_OFFSET)
#define TSS_BUSY (1 << TSS_BUSY_OFFSET)
#define TSS_TYPE 9

#define TSS_ABSENT_ACCESS (GDT_ABSENT | GDT_KERNEL | TSS_TYPE | TSS_AVAIL)
#define TSS_AVAIL_ACCESS (GDT_PRESENT | GDT_KERNEL | TSS_TYPE | TSS_AVAIL)
#define TSS_BUSY_ACCESS (GDT_PRESENT | GDT_KERNEL | TSS_TYPE | TSS_BUSY)

#define GDT_TSS_ZERO_ENTRY 9

typedef struct {
	uint16_t backlink, __blh;
	uint32_t esp0;
	uint16_t ss0, __ss0h;
	uint32_t esp1;
	uint16_t ss1, __ss1h;
	uint32_t esp2;
	uint16_t ss2, __ss2h;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es, __esh;
	uint16_t cs, __csh;
	uint16_t ss, __ssh;
	uint16_t ds, __dsh;
	uint16_t fs, __fsh;
	uint16_t gs, __gsh;
	uint16_t ldt, __ldth;
	uint16_t trace;
	uint16_t iomap;

} tss_t;

typedef struct {
	uint32_t old_ebp;
	uint32_t old_esp;
	uint32_t ret_address;
} caller_data_t;

/* ==================== functions ======================= */
uint32_t get_tss_limit();
void	 set_tss(tss_t* tss, void* fun);
void	 copy_registers_to_tss(tss_t* tss);
void	 fork_registers_to_tss(tss_t* tss);
void	 init_tss_registers(tss_t* tss, void* fun);
tss_t*	 get_tss_addr_by_gdt_offset(uint32_t offset);
void	 load_task_register(uint32_t offset);
uint16_t store_task_register();
void	 run_task_zero();
void	 switch_task(uint32_t index);
void	 task_switch(uint16_t gdt_offset);
void	 print_tss(tss_t* tss);
tss_t*	 spawn_tss(caller_data_t caller_data);
size_t	 gdt_add_tss_descriptor(tss_t* tss);
void	 remove_tss_descriptor(size_t index);

#endif
