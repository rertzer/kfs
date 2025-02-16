#ifndef PIC_H
#define PIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_ICW4 0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE 0x02	/* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08		/* Level triggered (edge) mode */
#define ICW1_INIT 0x10		/* Initialization - required! */

#define ICW4_8086 0x01		 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02		 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08	 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10		 /* Special fully nested (not) */

#define IRQ_TIMER 0
#define IRQ_KEYBOARD 1
#define MASKED 1
#define UNMASKED 0

void init_PIC();
void PIC_remap(uint8_t pic1_offset, uint8_t pic2_offset);
void IRQ_set_mask(uint8_t IRQline, bool masked);
void IRQ_unmask_all();
void IRQ_mask_all();
#endif
