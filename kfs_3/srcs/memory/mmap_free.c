#include "keycode.h"
#include "mmap.h"
#include "mmap_inline.h"
#include "printk.h"

static void			   fuse_chunk(mmap_t* mmap, chunk_t chunk);
static chunk_t		   get_parent(chunk_t kid);
static chunk_t		   get_buddy(mmap_t* mmap, chunk_t chunk);
static inline uint32_t get_buddy_offset(uint32_t offset);

uint8_t free_by_address(mmap_t* mmap, void const* const addr) {
	uint8_t	 ret = 0;
	uint32_t page_index = get_page_index(addr);

	printk("page index %u\n", page_index);
	press_any();
	chunk_t chunk = get_chunk(mmap, page_index);
	printk("chunk found\n");
	press_any();
	if (chunk.status == MMAP_USED || chunk.status == MMAP_USED_RONLY) {
		printk("free chunk found\n");
		chunk.status = MMAP_FREE;
		set_chunk_status(mmap, chunk);
		printk("status set\n");
		fuse_chunk(mmap, chunk);
		printk("fuse done\n");
	} else {
		ret = 1;
	}
	return (ret);
}

static void fuse_chunk(mmap_t* mmap, chunk_t chunk) {
	if (chunk.shift >= mmap->max_shift || chunk.status != MMAP_FREE) {
		return;
	}
	chunk_t buddy = get_buddy(mmap, chunk);
	if (buddy.status != MMAP_FREE) {
		return;
	}
	chunk.status = MMAP_UNAVAILABLE;
	buddy.status = MMAP_UNAVAILABLE;
	set_chunk_status(mmap, chunk);
	set_chunk_status(mmap, buddy);
	chunk = get_parent(chunk);

	set_chunk_status(mmap, chunk);
	fuse_chunk(mmap, chunk);
}

static chunk_t get_buddy(mmap_t* mmap, chunk_t chunk) {
	chunk_t buddy;
	buddy.shift = chunk.shift;
	buddy.byte = chunk.byte;
	buddy.offset = get_buddy_offset(chunk.offset);
	buddy.status = get_chunk_status(mmap, buddy);
	return (buddy);
}

static inline uint32_t get_buddy_offset(uint32_t offset) {
	if ((offset < 7) && (offset % 2 == 0)) {
		uint32_t toto = offset & (uint32_t)3;
		offset += toto == 0 ? BITS_PER_CHUNK : -BITS_PER_CHUNK;
	} else {
		offset = MMAP_NOT_FOUND_OFFSET;
	}
	return (offset);
}

static chunk_t get_parent(chunk_t kid) {
	uint32_t index = get_chunk_index(kid);
	// twice less entries one higher level
	index >>= 1;
	chunk_t parent;
	parent.shift = kid.shift + 1;
	parent.byte = get_byte(index);
	parent.offset = get_offset(index);
	parent.status = MMAP_FREE;

	return (parent);
}
