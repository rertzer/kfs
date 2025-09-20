#include "malloc.h"
#include "string.h"

t_block* get_next_block(t_block* block) {
	size_t size;

	if (block == NULL)
		return (NULL);
	size = UNFLAG(block->size);
	if (size == 0)
		return (NULL);
	size += SIZEOF_BLOCK;
	return (addr_offset(block, +size));
}

t_block* fragment_block(t_block* block, size_t size) {
	size_t	 flag;
	t_block* new_block;

	remove_block_from_bins(block);
	if (UNFLAG(block->size) == (SIZEOF_BLOCK + size))
		return (block);
	flag = (size_t)(block->size & 0b11);
	block->size = flag | (UNFLAG(block->size) - (SIZEOF_BLOCK + size));

	new_block = get_next_block(block);
	memset((uint8_t*)new_block, 0, SIZEOF_BLOCK);
	new_block->prev_block = block;
	new_block->size = size;
	new_block->page = block->page;
	return (new_block);
}

t_block* coalesce_block(t_block* block) {
	t_block* next;
	size_t	 new_size;

	if (block == NULL)
		return (NULL);
	block->size |= FREE;
	while (block->prev_block && block->prev_block->size & FREE)
		block = block->prev_block;

	remove_block_from_bins(block);

	new_size = UNFLAG(block->size);
	next = get_next_block(block);
	while (next && (next->size & FREE)) {
		new_size += SIZEOF_BLOCK + UNFLAG(next->size);
		remove_block_from_bins(next);
		next = get_next_block(next);
	}
	block->size = new_size | FREE;
	get_next_block(block)->prev_block = block;
	return (block);
}
