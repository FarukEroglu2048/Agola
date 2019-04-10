#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void pmm_set_frame(uint64_t frame_address);

uint64_t pmm_alloc_frame();
void pmm_free_frame(uint64_t frame_address);

void pmm_initialize(uint64_t memory_size);

#endif