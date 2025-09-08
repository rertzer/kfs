#include "malloc.h"

struct s_block bins[MAX_BINS] = {};

static size_t get_bin_index(size_t size)
{
    if (size > MAX_FAST_SIZE) size -= MAX_FAST_SIZE;
    size /= ALIGNMENT;
    return (size > 127 ? 127 : size);
}

t_block* remove_block_from_bins(t_block* block)
{
    if (!block) return(NULL);
    if (block->next) block->next->prev = block->prev;
    if (block->prev) block->prev->next = block->next;
    block->next = NULL;
    block->prev = NULL;
    return (block);
}

void    insert_after(t_block* current, t_block* new_block)
{
    if (!current || !new_block) return; 

    new_block->next = current->next;
    new_block->prev = current;

    if (current->next) current->next->prev = new_block;
    current->next = new_block;
}

void    release_block(t_block* block, t_alloc *alloc, t_fp_munbook unmmap)
{
    size_t  index;
    t_block *bin;

    if (block == NULL) return;
    block->page->block_count -= 1;
    if (block->page->block_count == 1) return (release_page(block->page, unmmap));
    index = get_bin_index(UNFLAG(block->size));
    bin = &alloc->bins[index];
    insert_after(bin, block);
}

t_block* request_block(size_t size, t_alloc *alloc, t_fp_mbook mmap)
{
    size_t  index;
    t_block* bin;
    t_page*  page;

    index = get_bin_index(size);
    bin = &alloc->bins[index];
    while (bin && UNFLAG(bin->size) < size) bin = bin->next;

    bin = remove_block_from_bins(bin);
    if (bin) {
        bin->size &= ~FREE;
        bin->page->block_count += 1;
        return (bin);
    }
    page = request_page(SIZEOF_BLOCK + size, alloc, mmap);
    if (page == NULL) return (NULL);
    bin = fragment_block(&page->free, size);
    if (bin) {
        bin->size &= ~FREE;
        bin->page->block_count += 1;
    }
    return (bin);
}
