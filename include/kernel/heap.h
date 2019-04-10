#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

uint64_t get_kernel_end();
void set_kernel_end(uint64_t address);

void* ksbrk(size_t increment);

void* allocate_memory(uint32_t size, uint32_t align);

void* kmalloc(uint32_t size);
void kfree(void* address);

void heap_initialize();

#endif