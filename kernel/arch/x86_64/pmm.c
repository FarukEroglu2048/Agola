#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel/heap.h>
#include <kernel/string.h>

#include <arch/x86_64/pmm.h>

uint64_t* frame_bitmap = NULL;

uint64_t frame_count = 0;
uint64_t used_frame_count = 0;

bool is_frame_used(uint64_t frame_address)
{
    uint64_t bitmap_entry_index = (frame_address / 0x1000) / 64;
    uint64_t entry_frame_index = (frame_address / 0x1000) % 64;

    return ((frame_bitmap[bitmap_entry_index] & (1 << entry_frame_index)) != 0) ? true : false;
}

void pmm_set_frame(uint64_t frame_address)
{
    if (is_frame_used(frame_address) == false)
    {
        uint64_t bitmap_entry_index = ((frame_address / 0x1000) / 64);
        uint64_t entry_frame_index = ((frame_address / 0x1000) % 64);

        frame_bitmap[bitmap_entry_index] |= (1 << entry_frame_index); 

        used_frame_count++;
    }
}

uint64_t pmm_alloc_frame()
{
    if (used_frame_count < frame_count)
    {
        uint64_t bitmap_entry_index = 0;

        while (frame_bitmap[bitmap_entry_index] == 0xFFFFFFFFFFFFFFFF) bitmap_entry_index++;

        uint64_t frame_address = bitmap_entry_index * 64 * 0x1000;

        while (is_frame_used(frame_address) == true) frame_address += 0x1000;

        pmm_set_frame(frame_address);
        
        return frame_address;
    }
    else return 0xFFFFFFFFFFFFFFFF;
}

void pmm_free_frame(uint64_t frame_address)
{
    if (is_frame_used(frame_address) == true)
    {
        uint64_t bitmap_entry_index = (frame_address / 0x1000) / 64;
        uint64_t entry_frame_index = (frame_address / 0x1000) % 64;

        frame_bitmap[bitmap_entry_index] &= ~(1 << entry_frame_index);

        used_frame_count--;
    }
}

void pmm_initialize(uint64_t memory_size)
{
    frame_count = memory_size / 0x1000;

    uint64_t frame_bitmap_size = frame_count / 8;

    uint64_t kernel_end = get_kernel_end();

    frame_bitmap = (uint64_t*)kernel_end;

    memset(frame_bitmap, 0x00, frame_bitmap_size);

    set_kernel_end(kernel_end + frame_bitmap_size);

    kernel_end = get_kernel_end();

    for (uint64_t frame_address = 0; frame_address <= (kernel_end - 0xFFFFFFFF80000000); frame_address += 0x1000) pmm_set_frame(frame_address);
}