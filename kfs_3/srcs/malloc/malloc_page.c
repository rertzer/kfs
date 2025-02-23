#include "malloc.h"

t_page  *insert_after_page(t_page *pages, t_page *page)
{
    page->prev = pages;
    page->next = pages->next;
    if (page->next) page->next->prev = page;
    pages->next = page;
    return (pages->next);
}

static size_t   compute_page_size(size_t request_size)
{
    size_t page_size;
    size_t num_pages;
    
    page_size = PAGE_SIZE;
    num_pages = (request_size + page_size - 1) / page_size;
    return (num_pages * page_size);
}

static t_page*   new_page(size_t size, t_fp_mbook mmap)
{
    t_page*  page;
    size_t  page_size;

    page_size = compute_page_size(SIZEOF_METADATA_PAGE + size);
    page = mmap(page_size);
    if (page == NULL) return (NULL);
    ft_memset((uint8_t*)page, 0, SIZEOF_PAGE);
    page->block_count = 1;
    page->free.page = page;
    page->free.size = (page_size - SIZEOF_METADATA_PAGE) | FREE;
    page->free.prev_block = NULL;
    return (page);
}

t_page*   request_page(size_t size, t_alloc *alloc, t_fp_mbook mmap)
{
    t_page* page;

    page = alloc->pages.next;
    while (page && UNFLAG(page->free.size) < size) page = page->next;
    if (page != NULL) return (page);
    page = new_page(size, mmap);
    if (page == NULL) return (NULL);
    return (insert_after_page(&alloc->pages, page));
}

void    release_page(t_page* page, t_fp_munbook unmmap)
{
    if (page->next) page->next->prev = page->prev;
    if (page->prev) page->prev->next = page->next;
    unmmap((void*)page);
}