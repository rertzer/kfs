#include "boot_infos.h"
#include "builtin.h"
#include "kernel.h"
#include "mmap.h"

extern uint32_t multiboot_magic;
extern uint32_t multiboot_tags;

static multiboot_addr_t get_multiboot_addr();
static void				mmmp_print_entries(multiboot_memory_map_t* mmmp, multiboot_info_t* mbd);
static inline size_t	mmmp_entry_end(multiboot_info_t* mbd);
static uint32_t			mmmp_print_entry(multiboot_memory_map_t* mmmp_entry);
static void				print_multiboot_magic();
static uint8_t*			get_upper_string(uint32_t addr);
static void				print_multiboot_infos(multiboot_addr_t* mba);

void boot_infos_memory_map_freeze(mmap_t* mmap) {
	multiboot_addr_t mba = get_multiboot_addr();

	for (size_t i = 0; i < mmmp_entry_end(mba.mbd); ++i) {
		if (mba.mmmp[i].type != MULTIBOOT_MEMORY_AVAILABLE) {
			book_memory(mmap, (uint8_t*)(uint32_t)mba.mmmp[i].addr, (uint32_t)mba.mmmp[i].len, MMAP_UNAVAILABLE);
		}
	}
}

// value in Kib
uint32_t boot_infos_get_mem_size() {
	multiboot_addr_t mba = get_multiboot_addr();
	return (mba.mbi[1] + mba.mbi[2]);
}

void memory_map_infos() {
	multiboot_addr_t mba = get_multiboot_addr();
	mmmp_print_entries(mba.mmmp, mba.mbd);
}

static multiboot_addr_t get_multiboot_addr() {
	multiboot_addr_t mba;

	uint32_t multiboot_infos_addr = *to_upper_kernel(&multiboot_tags);
	mba.mbi = to_upper_kernel((uint32_t*)multiboot_infos_addr);
	mba.mbd = (multiboot_info_t*)to_upper_kernel((uint32_t*)multiboot_infos_addr);
	mba.mmmp = (multiboot_memory_map_t*)to_upper_kernel((uint32_t*)(mba.mbd->mmap_addr));

	return (mba);
}

static void mmmp_print_entries(multiboot_memory_map_t* mmmp, multiboot_info_t* mbd) {
	uint32_t total_size = 0;

	for (size_t i = 0; i < mmmp_entry_end(mbd); ++i) {
		total_size += mmmp_print_entry(&mmmp[i]);
	}
	printk("total size: %u\n", total_size);
}

static inline size_t mmmp_entry_end(multiboot_info_t* mbd) {
	return (mbd->mmap_length / sizeof(multiboot_memory_map_t));
}

static uint32_t mmmp_print_entry(multiboot_memory_map_t* mmmp_entry) {
	char const* is_avail = (mmmp_entry->type == MULTIBOOT_MEMORY_AVAILABLE) ? "available" : "unavailable";
	printk("memory addr: %08x, length %u:\t%s\n", (uint32_t)mmmp_entry->addr, (uint32_t)mmmp_entry->len, is_avail);
	return (mmmp_entry->len);
}

uint8_t boot_infos(char* pointer, size_t len) {
	(void)pointer;
	(void)len;

	print_multiboot_magic();
	multiboot_addr_t mba = get_multiboot_addr();
	print_multiboot_infos(&mba);
	mmmp_print_entries(mba.mmmp, mba.mbd);
	return (0);
}

static void print_multiboot_magic() {
	uint32_t magic = *to_upper_kernel(&multiboot_magic);
	printk("multiboot magic: %08x", magic);
	if (magic == MULTIBOOT_MAGIC_1) {
		printk(" (multiboot V1)\n");
	} else if (magic == MULTIBOOT_MAGIC_2) {
		printk(" (multiboot V2)\n");
	} else {
		printk(" (BAD VALUE\n)");
	}
}

static void print_multiboot_infos(multiboot_addr_t* mba) {
	uint8_t*  multiboot_cmdline = get_upper_string(mba->mbi[4]);
	uint8_t*  multiboot_bootloader = get_upper_string(mba->mbi[16]);
	uint32_t  multiboot_mmap_length = mba->mbi[11];
	uint32_t* multiboot_mmap_addr = to_upper_kernel((uint32_t*)mba->mbi[12]);

	printk("flags:  %08x\n", mba->mbi[0]);
	printk("mem_lower:  %u KiB (max 640)\n", mba->mbi[1]);
	printk("mem_upper:  %08x\n", mba->mbi[2]);
	printk("boot_device:  %08x\n", mba->mbi[3]);
	printk("cmdline:  %s\n", multiboot_cmdline);
	printk("elf num:  %08x, ", mba->mbi[7]);
	printk("elf size:  %08x\n", mba->mbi[8]);
	printk("elf addr:  %08x, ", mba->mbi[9]);
	printk("elf shndx:  %08x\n", mba->mbi[10]);
	printk("bootloader:  %s\n", multiboot_bootloader);
	printk("mmap length:  %08x, ", multiboot_mmap_length);
	printk("mmap addr:  %08x\n", multiboot_mmap_addr);
}

static uint8_t* get_upper_string(uint32_t addr) {
	return ((uint8_t*)to_upper_kernel((uint32_t*)addr));
}
