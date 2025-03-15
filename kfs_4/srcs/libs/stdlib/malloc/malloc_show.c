#include "malloc.h"
#include "printk.h"


static t_block*  show_block_info(t_block* block)
{
    size_t  size;

    if (UNFLAG(block->size) == 0) return (NULL);
    size = UNFLAG(block->size);
    printk("[%d>%d] ", size, SIZEOF_BLOCK + size);
    return (get_next_block(block));
}

static t_page*   show_page_info(t_page* page)
{
    t_block* block;
    size_t  index;

    printk("page addr: %x |", page);
    printk(" space: %d |", UNFLAG(page->free.size) - SIZEOF_PAGE);
    printk(" block count: %d\n", page->block_count);

    block = &page->free;
    index = 0;
    while (block)
    {
        block = show_block_info(block);
        if (index == 8) {
            printk("\n");
            index = 0;
        }
        index += 1;
    }
    printk("\n");
    return (page->next);
}

void    show_alloc_mem(t_alloc* alloc)
{
	t_page*  page;

    page = &alloc->pages;
    if (page == NULL) {
        printk("%s \n", "No page left");
        return;
    }
	while (page) page = show_page_info(page);
}