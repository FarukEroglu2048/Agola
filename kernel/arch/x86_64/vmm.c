#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <arch/x86_64/pmm.h>

#include <kernel/heap.h>
#include <kernel/string.h>

#include <arch/x86_64/vmm.h>

#define PAGE_FLAG_PRESENT 0x001
#define PAGE_FLAG_RW 0x002
#define PAGE_FLAG_USER 0x004

#define PAGE_DIRECTORY_TABLE_OFFSET 39
#define PAGE_DIRECTORY_OFFSET 30
#define PAGE_TABLE_OFFSET 21
#define PAGE_OFFSET 12

#define KERNEL_PAGE_DIRECTORY_TABLE_INDEX 511
#define CURRENT_RECURSIVE_MAP_INDEX 510
#define SECONDARY_RECURSIVE_MAP_INDEX 509

extern void invalidate_page(uint64_t page_address);
extern void invalidate_pml4_table();

uint64_t* get_virtual_page(size_t page_directory_table_index, size_t page_directory_index, size_t page_table_index, size_t page_index)
{
    uint64_t page_address = (page_directory_table_index << PAGE_DIRECTORY_TABLE_OFFSET) | (page_directory_index << PAGE_DIRECTORY_OFFSET) | (page_table_index << PAGE_TABLE_OFFSET) | (page_index << PAGE_OFFSET);

    if (page_address >= 0x800000000000) page_address |= 0xFFFF000000000000;

    return (uint64_t*)page_address;
}

uint64_t* get_pml4_table(bool secondary_recursive_map)
{
    if (secondary_recursive_map == true) return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, SECONDARY_RECURSIVE_MAP_INDEX);
    else return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX);
}

uint64_t* get_page_directory_table(bool secondary_recursive_map, size_t page_directory_table_index)
{
    if (secondary_recursive_map == true) return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, SECONDARY_RECURSIVE_MAP_INDEX, page_directory_table_index);
    else return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, page_directory_table_index);
}

uint64_t* get_page_directory(bool secondary_recursive_map, size_t page_directory_table_index, size_t page_directory_index)
{
    if (secondary_recursive_map == true) return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, SECONDARY_RECURSIVE_MAP_INDEX, page_directory_table_index, page_directory_index);
    else return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, CURRENT_RECURSIVE_MAP_INDEX, page_directory_table_index, page_directory_index);
}

uint64_t* get_page_table(bool secondary_recursive_map, size_t page_directory_table_index, size_t page_directory_index, size_t page_table_index)
{
    if (secondary_recursive_map == true) return get_virtual_page(SECONDARY_RECURSIVE_MAP_INDEX, page_directory_table_index, page_directory_index, page_table_index);
    else return get_virtual_page(CURRENT_RECURSIVE_MAP_INDEX, page_directory_table_index, page_directory_index, page_table_index);
}

void set_secondary_recursive_map(uint64_t pml4_table_physical_address)
{
    uint64_t* current_pml4_table = get_pml4_table(false);
    uint64_t* secondary_pml4_table = get_pml4_table(true);

    current_pml4_table[SECONDARY_RECURSIVE_MAP_INDEX] = pml4_table_physical_address | PAGE_FLAG_PRESENT | PAGE_FLAG_RW;

    invalidate_page((uint64_t)secondary_pml4_table);
}

void remove_secondary_recursive_map()
{
    uint64_t* current_pml4_table = get_pml4_table(false);
    uint64_t* secondary_pml4_table = get_pml4_table(true);

    current_pml4_table[SECONDARY_RECURSIVE_MAP_INDEX] = 0x0000000000000000;

    invalidate_page((uint64_t)secondary_pml4_table);   
}

uint64_t create_pml4_table(bool map_kernel, bool set_as_secondary)
{
    uint64_t pml4_table_physical_address = pmm_alloc_frame();

    set_secondary_recursive_map(pml4_table_physical_address);

    uint64_t* current_pml4_table = get_pml4_table(false);
    uint64_t* new_pml4_table = get_pml4_table(true);

    memset(new_pml4_table, 0x00, 0x1000);

    new_pml4_table[CURRENT_RECURSIVE_MAP_INDEX] = pml4_table_physical_address | PAGE_FLAG_PRESENT | PAGE_FLAG_RW;
    if (map_kernel == true) new_pml4_table[KERNEL_PAGE_DIRECTORY_TABLE_INDEX] = current_pml4_table[KERNEL_PAGE_DIRECTORY_TABLE_INDEX];

    if (set_as_secondary == false) remove_secondary_recursive_map();

    return pml4_table_physical_address;
}

uint64_t get_physical_address(bool secondary_recursive_map, uint64_t virtual_address)
{
    size_t page_directory_table_index = ((virtual_address >> PAGE_DIRECTORY_TABLE_OFFSET) & 0x1FF);
    size_t page_directory_index = ((virtual_address >> PAGE_DIRECTORY_OFFSET) & 0x1FF);
    size_t page_table_index = ((virtual_address >> PAGE_TABLE_OFFSET) & 0x1FF);
    size_t page_index = ((virtual_address >> PAGE_OFFSET) & 0x1FF);
    
    uint64_t* pml4_table = get_pml4_table(secondary_recursive_map);
    uint64_t* page_directory_table = get_page_directory_table(secondary_recursive_map, page_directory_table_index);
    uint64_t* page_directory = get_page_directory(secondary_recursive_map, page_directory_table_index, page_directory_index);
    uint64_t* page_table = get_page_table(secondary_recursive_map, page_directory_table_index, page_directory_index, page_table_index);
    
    if (pml4_table[page_directory_table_index] != 0)
    {
        if (page_directory_table[page_directory_index] != 0)
        {
            if (page_directory[page_table_index] != 0)
            {
                return ((page_table[page_index] & (~0xFFF)) | (virtual_address & 0xFFF));
            }
        }
    }

    return 0xFFFFFFFFFFFFFFFF;
}

void vmm_unmap_page(bool secondary_recursive_map, uint64_t virtual_address, bool free_frame)
{
    size_t page_directory_table_index = ((virtual_address >> PAGE_DIRECTORY_TABLE_OFFSET) & 0x1FF);
    size_t page_directory_index = ((virtual_address >> PAGE_DIRECTORY_OFFSET) & 0x1FF);
    size_t page_table_index = ((virtual_address >> PAGE_TABLE_OFFSET) & 0x1FF);
    size_t page_index = ((virtual_address >> PAGE_OFFSET) & 0x1FF);
    
    uint64_t* pml4_table = get_pml4_table(secondary_recursive_map);
    uint64_t* page_directory_table = get_page_directory_table(secondary_recursive_map, page_directory_table_index);
    uint64_t* page_directory = get_page_directory(secondary_recursive_map, page_directory_table_index, page_directory_index);
    uint64_t* page_table = get_page_table(secondary_recursive_map, page_directory_table_index, page_directory_index, page_table_index);
    
    if (pml4_table[page_directory_table_index] != 0)
    {  
        if (page_directory_table[page_directory_index] != 0)
        {
            if (page_directory[page_table_index] != 0)
            {
                if (page_table[page_index] != 0)
                {
                    if (free_frame == true) pmm_free_frame(page_table[page_index] & ~0xFFF);

                    page_table[page_index] = 0;

                    invalidate_page(virtual_address);
                }
            }
        }
    }
}

void vmm_map_page(bool secondary_recursive_map, uint64_t virtual_address, uint64_t physical_address, bool user, bool rw, bool free_frame)
{
    size_t page_directory_table_index = ((virtual_address >> PAGE_DIRECTORY_TABLE_OFFSET) & 0x1FF);
    size_t page_directory_index = ((virtual_address >> PAGE_DIRECTORY_OFFSET) & 0x1FF);
    size_t page_table_index = ((virtual_address >> PAGE_TABLE_OFFSET) & 0x1FF);
    size_t page_index = ((virtual_address >> PAGE_OFFSET) & 0x1FF);
    
    uint64_t* pml4_table = get_pml4_table(secondary_recursive_map);
    uint64_t* page_directory_table = get_page_directory_table(secondary_recursive_map, page_directory_table_index);
    uint64_t* page_directory = get_page_directory(secondary_recursive_map, page_directory_table_index, page_directory_index);
    uint64_t* page_table = get_page_table(secondary_recursive_map, page_directory_table_index, page_directory_index, page_table_index);
    
    if (pml4_table[page_directory_table_index] == 0)
    {
        pml4_table[page_directory_table_index] = (pmm_alloc_frame() | PAGE_FLAG_PRESENT);

        invalidate_page((uint64_t)page_directory_table);

        memset(page_directory_table, 0x00, 0x1000);

        if (user == true) pml4_table[page_directory_table_index] |= PAGE_FLAG_USER; 
        if (rw == true) pml4_table[page_directory_table_index] |= PAGE_FLAG_RW;
    }

    if (page_directory_table[page_directory_index] == 0)
    {
        page_directory_table[page_directory_index] = (pmm_alloc_frame() | PAGE_FLAG_PRESENT);

        invalidate_page((uint64_t)page_directory);

        memset(page_directory, 0x00, 0x1000);

        if (user == true) page_directory_table[page_directory_index] |= PAGE_FLAG_USER;
        if (rw == true) page_directory_table[page_directory_index] |= PAGE_FLAG_RW;
    }

    if (page_directory[page_table_index] == 0)
    {
        page_directory[page_table_index] = (pmm_alloc_frame() | PAGE_FLAG_PRESENT);

        invalidate_page((uint64_t)page_table);

        memset(page_table, 0x00, 0x1000);

        if (user == true) page_directory[page_table_index] |= PAGE_FLAG_USER;
        if (rw == true) page_directory[page_table_index] |= PAGE_FLAG_RW;
    }

    if (page_table[page_index] != 0)
    {
        if (free_frame == true) pmm_free_frame(page_table[page_index] & ~0xFFF);

        page_table[page_index] = 0;
    }

    page_table[page_index] = physical_address;

    page_table[page_index] |= PAGE_FLAG_PRESENT;

    if (user == true) page_table[page_index] |= PAGE_FLAG_USER; 
    if (rw == true) page_table[page_index] |= PAGE_FLAG_RW;

    invalidate_page(virtual_address);
}

void vmm_map_memory(bool secondary_recursive_map, uint64_t virtual_address, uint64_t size, bool user, bool rw)
{
    for (uint64_t current_address = virtual_address; current_address < (virtual_address + size); current_address += 0x1000)
    {
        vmm_map_page(secondary_recursive_map, current_address, pmm_alloc_frame(), user, rw, true);
    }
}

void vmm_unmap_memory(bool secondary_recursive_map, uint64_t virtual_address, uint64_t size, bool free_frames)
{
    for (uint64_t current_address = virtual_address; current_address < (virtual_address + size); current_address += 0x1000)
    {
        vmm_unmap_page(secondary_recursive_map, current_address, free_frames);
    }
}

void vmm_initialize()
{
    uint64_t pml4_table_physical_address = create_pml4_table(false, true);

    uint64_t kernel_end = get_kernel_end();

    for (uint64_t virtual_address = 0xFFFFFFFF80000000; virtual_address <= kernel_end; virtual_address += 0x1000)
    {
        vmm_map_page(true, virtual_address, virtual_address - 0xFFFFFFFF80000000, false, true, true);
    }

    switch_pml4_table(pml4_table_physical_address);
}