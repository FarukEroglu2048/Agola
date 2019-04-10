#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <stdbool.h>

extern uint64_t get_current_pml4_table();
extern void switch_pml4_table(uint64_t pml4_table_physical_address);

void set_secondary_recursive_map(uint64_t pml4_table_physical_address);
void remove_secondary_recursive_map();

uint64_t create_pml4_table(bool map_kernel, bool set_as_secondary);

uint64_t get_physical_address(bool secondary_recursive_map, uint64_t virtual_address);

void vmm_map_page(bool secondary_recursive_map, uint64_t virtual_address, uint64_t physical_address, bool user, bool rw, bool free_frame);
void vmm_unmap_page(bool secondary_recursive_map, uint64_t virtual_address, bool free_frame);

void vmm_map_memory(bool secondary_recursive_map, uint64_t virtual_address, uint64_t size, bool user, bool rw);
void vmm_unmap_memory(bool secondary_recursive_map, uint64_t virtual_address, uint64_t size, bool free_frames);

void vmm_initialize();

#endif