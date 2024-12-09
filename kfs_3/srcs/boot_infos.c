#include "builtin.h"
#include "kernel.h"

extern uint32_t multiboot_magic;
extern uint32_t multiboot_tags;

uint8_t boot_infos() {
	uint32_t  magic = *to_upper_kernel(&multiboot_magic);
	uint32_t  multiboot_infos_addr = *to_upper_kernel(&multiboot_tags);
	uint32_t* multiboot_infos = to_upper_kernel((uint32_t*)multiboot_infos_addr);
	uint8_t*  multiboot_cmdline = (uint8_t*)to_upper_kernel((uint32_t*)multiboot_infos[4]);
	uint8_t*  multiboot_bootloader = (uint8_t*)to_upper_kernel((uint32_t*)multiboot_infos[16]);
	uint32_t  multiboot_mmap_length = multiboot_infos[11];
	uint32_t* multiboot_mmap_addr = to_upper_kernel((uint32_t*)multiboot_infos[12]);
	printk("multiboot magic: 0x%08x", magic);
	if (magic == MULTIBOOT_MAGIC_1) {
		printk(" (multiboot V1)\n");
	} else if (magic == MULTIBOOT_MAGIC_2) {
		printk(" (multiboot V2)\n");
	} else {
		printk(" (BAD VALUE\n)");
	}

	printk("flags:  0x%08x\n", multiboot_infos[0]);
	printk("mem_lower:  %u KiB (max 640)\n", multiboot_infos[1]);
	printk("mem_upper:  0x%08x\n", multiboot_infos[2]);
	printk("boot_device:  0x%08x\n", multiboot_infos[3]);
	printk("cmdline:  %s\n", multiboot_cmdline);
	printk("elf num:  0x%08x\n", multiboot_infos[7]);
	printk("elf size:  0x%08x\n", multiboot_infos[8]);
	printk("elf addr:  0x%08x\n", multiboot_infos[9]);
	printk("elf shndx:  0x%08x\n", multiboot_infos[10]);
	printk("bootloader:  %s\n", multiboot_bootloader);
	printk("mmap length:  0x%08x\n", multiboot_mmap_length);
	printk("mmap addr:  0x%08x\n", multiboot_mmap_addr);
	uint32_t i = 0;
	while (i < multiboot_mmap_length) {
		printk("base address 1: 0x%08x\t", multiboot_mmap_addr[i]);
		++i;
		printk("base address 2: 0x%08x\n", multiboot_mmap_addr[i]);
		++i;
		printk("size 1: 0x%08x\t", multiboot_mmap_addr[i]);
		++i;
		printk("size 2: 0x%08x\n", multiboot_mmap_addr[i]);
		++i;
		printk("type: 0x%08x\n", multiboot_mmap_addr[i]);
		++i;
		printk("????: 0x%08x\n", multiboot_mmap_addr[i]);
		++i;
	}
	printk("prev: 0x%08x\t", multiboot_mmap_addr[-1]);
	return (0);
}
