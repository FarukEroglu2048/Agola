#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stdbool.h>

#define DESCRIPTOR_PRIVILEGE_LEVEL_0 0b00
#define DESCRIPTOR_PRIVILEGE_LEVEL_1 0b01
#define DESCRIPTOR_PRIVILEGE_LEVEL_2 0b10
#define DESCRIPTOR_PRIVILEGE_LEVEL_3 0b11

void gdt_create_null_entry(uint16_t index);
void gdt_create_long_entry(uint16_t index, uint8_t privilege_level, bool executable);

void gdt_initialize();

#endif