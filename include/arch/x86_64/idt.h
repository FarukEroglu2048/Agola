#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_create_null_entry(uint8_t index);
void idt_create_interrupt_entry(uint8_t index, uint8_t privilege_level, uint64_t base);

void idt_initialize();

#endif