#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <arch/x86_64/vmm.h>

#define KERNEL_HEAP_START 0xFFFFFFFF8A000000
#define KMALLOC_ALIGN 16

typedef struct allocation_header
{
    uint32_t free_size;

    bool used;
} allocation_header_t;

typedef struct allocation_footer
{
    uint32_t pad_size;
} allocation_footer_t;

extern uint8_t kernel_end_symbol;
uint64_t kernel_end = (uint64_t)(&kernel_end_symbol);

uint64_t heap_start = 0;

uint64_t align_value(uint64_t value, size_t align)
{
    return (value + (align - 1)) & ~(align - 1);
}

uint64_t get_kernel_end()
{
    return kernel_end;
}

void set_kernel_end(uint64_t address)
{
    kernel_end = address;
}

void* ksbrk(size_t increment)
{
    uint64_t kernel_end_old = kernel_end;

    kernel_end += increment;

    if (get_physical_address(false, kernel_end) == 0xFFFFFFFFFFFFFFFF)
    {
        vmm_map_memory(false, kernel_end, 0x1000, false, true);
    }

    return (void*)kernel_end_old;
}

void* allocate_memory(uint32_t size, uint32_t align)
{
    if (align < KMALLOC_ALIGN) align = KMALLOC_ALIGN;
    size = align_value(size, align);

    uint64_t heap_pointer = heap_start;

    while(heap_pointer < heap_start + 0x100000)
    {
        allocation_header_t* allocation_header = (allocation_header_t*)heap_pointer;

        heap_pointer += sizeof(allocation_header_t);

        uint32_t pad_size = align_value(heap_pointer + sizeof(allocation_footer_t), align) - (heap_pointer + sizeof(allocation_footer_t));

        if (allocation_header->free_size == 0 || (allocation_header->free_size >= size + pad_size && allocation_header->used == false))
        {
            if (allocation_header->free_size == 0) allocation_header->free_size = size + pad_size; 
            allocation_header->used = true;

            heap_pointer += pad_size;

            allocation_footer_t* allocation_footer = (allocation_footer_t*)heap_pointer;

            allocation_footer->pad_size = pad_size;

            return (void*)(heap_pointer + sizeof(allocation_footer_t));
        }
        else
        {
            heap_pointer += allocation_header->free_size + sizeof(allocation_footer_t);
        }
    }

    return NULL;
}

void* kmalloc(uint32_t size)
{
    return allocate_memory(size, KMALLOC_ALIGN);
}

void kfree(void* address)
{
    allocation_footer_t* allocation_footer = (allocation_footer_t*)(address - sizeof(allocation_footer_t));
    allocation_header_t* allocation_header = (allocation_header_t*)(address - sizeof(allocation_footer_t) - allocation_footer->pad_size - sizeof(allocation_header_t));

    allocation_header->used = false;
}

void heap_initialize()
{
    vmm_map_memory(false, KERNEL_HEAP_START, 0x100000, false, true);

    heap_start = KERNEL_HEAP_START;
}